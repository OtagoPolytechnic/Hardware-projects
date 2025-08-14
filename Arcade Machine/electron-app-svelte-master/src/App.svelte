<script>
	import { onMount } from 'svelte';
	import SimpleCarousel from './components/SimpleCarousel.svelte';

	let games = [];
	let isLoading = true;

	onMount(async () => {
		await loadGames();
	});

	async function loadGames() {
		try {
			games = await window.electronAPI.getGamesFromFolder();
			isLoading = false;
		} catch (error) {
			console.error('Failed to load games:', error);
			isLoading = false;
		}
	}
</script>

	<main class="app-container" >
		<h1>Arcade</h1>
		{#if isLoading}
			<div class="loading">Loading...</div>
		{/if}

		{#if !isLoading && games.length === 0}
			<div class="no-games">No games found. Please add games to the folder.</div>
		{/if}

	
			<SimpleCarousel {games} />

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