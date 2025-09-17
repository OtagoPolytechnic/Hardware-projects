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
</style>
