<script>
	import { onMount } from 'svelte';
	import SimpleCarousel from './components/SimpleCarousel.svelte';
	import SettingsBar from './components/SettingsBar.svelte';

	let games = [];
	let isLoading = true;
	let gamesFolder = '';
	let retroarchFolder = '';

	onMount(async () => {
		await loadGames();

	});

	async function loadGames() {
		try {
			gamesFolder = await window.electronAPI.getGamesFolderPath();
			retroarchFolder = await window.electronAPI.getRetroarchPath();
			games = await window.electronAPI.getGamesFromFolder(gamesFolder);
			isLoading = false;
		} catch (error) {
			console.error('Failed to load games:', error);
			isLoading = false;
		}
	}

	async function chooseGamesFolder() {
		const folder = await window.electronAPI.chooseGamesFolder();
		if (folder) {
			gamesFolder = folder;
			await loadGames();
		}
	}

	async function chooseRetroarchPath() {
		const path = await window.electronAPI.chooseRetroarchPath();
		if (path) {
			retroarchFolder = path;
			// Trigger a refresh of the carousel component to update supported ROMs
			await loadGames();
		}
	}
</script>
<main class="app-container" >
	<SettingsBar
		{gamesFolder}
		{retroarchFolder}
		on:chooseGamesFolder={chooseGamesFolder}
		on:chooseRetroarchPath={chooseRetroarchPath}
	/>
	<h1>Arcade</h1>
		{#if isLoading}
			<div class="loading">Loading...</div>
		{/if}


		{#if !isLoading && games.length === 0}
			<p class="no-games">No games found. Please add games to the Desktop/Games folder.</p>
			<p>if you want cover art add a image with the same name as the game to the folder</p>

		{/if}
		
	 	<SimpleCarousel {games} />


	</main>
<style>
	.app-container {
		background: #1a1a2e;
		color: #fff;
		padding: clamp(0.5em, 2vw, 2em);
		font-family: Arial, sans-serif;
		outline: none;
		min-height: 100vh;
		box-sizing: border-box;
		overflow-x: hidden;
	}

	main {
		text-align: center;
		max-width: 100%;
	}

	h1 {
		margin-bottom: clamp(1em, 3vh, 2em);
		font-size: clamp(1.5rem, 5vw, 2.5rem);
	}

	.loading, .no-games {
		font-size: clamp(1rem, 3vw, 1.5rem);
		margin-top: clamp(1em, 3vh, 2em);
		padding: 0 1em;
	}

	@media (max-width: 768px) {
		.app-container {
			padding: 0.5em;
		}
	}
</style>