<script>
	import { createEventDispatcher } from 'svelte';
	export let gamesFolder = '';
	export let retroarchFolder = '';
	const dispatch = createEventDispatcher();

	function handleChooseGamesFolder() {
		dispatch('chooseGamesFolder');
	}
	function handleChooseRetroarchPath() {
		dispatch('chooseRetroarchPath');
	}
</script>

<div class="settings-bar">
	<div>
		<label for="games-folder-input">Games Folder:</label>
		<input id="games-folder-input" type="text" value={gamesFolder} readonly />
		<button on:click={handleChooseGamesFolder}>Choose Folder</button>
	</div>
	<div>
		<label for="retroarch-path-input">RetroArch Path:</label>
		<input id="retroarch-path-input" type="text" value={retroarchFolder} readonly />
		<button on:click={handleChooseRetroarchPath}>Choose Path</button>
	</div>
</div>

<style>
	.settings-bar {
		display: flex;
		flex-wrap: wrap;
		gap: clamp(0.5em, 2vw, 2em);
		margin-bottom: clamp(1em, 3vh, 2em);
		justify-content: center;
		align-items: center;
		padding: 0 1em;
	}

	.settings-bar > div {
		display: flex;
		flex-wrap: wrap;
		gap: 0.5em;
		align-items: center;
		justify-content: center;
		min-width: 0;
	}

	.settings-bar label {
		font-weight: bold;
		white-space: nowrap;
		font-size: clamp(0.8rem, 2vw, 1rem);
	}

	.settings-bar input {
		width: clamp(150px, 30vw, 260px);
		max-width: 100%;
		padding: 0.3em;
		border-radius: 4px;
		border: 1px solid #444;
		background: #222;
		color: #fff;
		font-size: clamp(0.75rem, 1.5vw, 0.9rem);
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
		white-space: nowrap;
		font-size: clamp(0.75rem, 1.5vw, 0.9rem);
	}

	.settings-bar button:hover {
		background: #00bfff;
	}

	@media (max-width: 768px) {
		.settings-bar {
			flex-direction: column;
			gap: 1em;
		}

		.settings-bar > div {
			width: 100%;
		}

		.settings-bar input {
			width: 100%;
		}
	}
</style>
