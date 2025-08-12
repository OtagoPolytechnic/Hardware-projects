<script>
  export let activeIndex;
  export let totalGames;
  export let onLaunch;
</script>

<div class="controls">
  <p>Game {activeIndex + 1} of {totalGames}</p>
  <button class="launch-btn" on:click={onLaunch}>Launch Game</button>
  <p>Use ← → to navigate, Enter to launch</p>
</div>

<style>
  .controls {
    text-align: center;
    margin-top: 2em;
  }

  .launch-btn {
    background: #ff6b35;
    color: white;
    border: none;
    padding: 1em 2em;
    font-size: 1.2rem;
    cursor: pointer;
    border-radius: 5px;
    margin: 1em 0;
  }

  .launch-btn:hover {
    background: #e55a2b;
  }
</style>
