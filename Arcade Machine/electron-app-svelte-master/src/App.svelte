<script>
	import { onMount } from 'svelte';
	import SimpleCarousel from './components/SimpleCarousel.svelte';

	let games = [];
	let isLoading = true;
	let gamesFolder = '';
	let retroarchPath = '';

	onMount(async () => {
		await loadGames();
		gamesFolder = await window.electronAPI.getGamesFolderPath();
		retroarchPath = await window.electronAPI.getRetroarchPath();
	});

	async function loadGames() {
		try {
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
			retroarchPath = path;
		}
	}
</script>
	<main class="app-container" >
		<div class="settings-bar">
			<div>
				<label>Games Folder:</label>
				<input type="text" bind:value={gamesFolder} readonly />
				<button on:click={chooseGamesFolder}>Choose Folder</button>
			</div>
			<div>
				<label>RetroArch Path:</label>
				<input type="text" bind:value={retroarchPath} readonly />
				<button on:click={chooseRetroarchPath}>Choose Path</button>
			</div>
		</div>
		<h1>Arcade</h1>
		{#if isLoading}
			<div class="loading">Loading...</div>
		{/if}


		{#if !isLoading && games.length === 0}
			<p class="no-games">No games found. Please add games to the Desktop/Games folder.</p>
			<p>if you want cover art add a image with the same name as the game to the folder</p>

		{/if}
		
	 	<SimpleCarousel {games} />

		<!-- nav  -->

	</main>
<style>
	.app-container {
		background: #1a1a2e;
		color: #fff;
		min-height: 100vh;
		padding: 2em;
		font-family: Arial, sans-serif;
		outline: none;
	}

	.settings-bar {
		display: flex;
		gap: 2em;
		margin-bottom: 2em;
		justify-content: center;
		align-items: center;
	}
	.settings-bar label {
		font-weight: bold;
		margin-right: 0.5em;
	}
	.settings-bar input {
		width: 260px;
		padding: 0.3em;
		margin-right: 0.5em;
		border-radius: 4px;
		border: 1px solid #444;
		background: #222;
		color: #fff;
	}
	.settings-bar button {
		background: #00ffff;
		color: #1a1a2e;
		border: none;
		border-radius: 4px;
		padding: 0.4em 1em;
		font-weight: bold;
		cursor: pointer;
		transition: background 0.2s;
	}
	.settings-bar button:hover {
		background: #00bfff;
	}

	main {
		width: 100%;
		height: 100%;
		text-align: center;
	}

	h1 {
		margin-bottom: 2em;
		font-size: 2.5rem;
	}

	.loading, .no-games {
		font-size: 1.5rem;
		margin-top: 2em;
	}
</style>