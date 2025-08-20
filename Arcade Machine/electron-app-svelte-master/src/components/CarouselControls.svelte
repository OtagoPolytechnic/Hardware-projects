<script>
  export let activeIndex;
  export let totalGames;
  export let onLaunch;
</script>

<div class="controls">
  <p class="game-counter">Game {activeIndex + 1} of {totalGames}</p>
  <button class="launch-btn" on:click={onLaunch}>
    <span class="btn-text">LAUNCH GAME</span>
    <div class="btn-glow"></div>
  </button>
  <p class="instructions">Use ← → to navigate, Enter to launch</p>
</div>

<style>
  @import url('https://fonts.googleapis.com/css2?family=Orbitron:wght@400;700&display=swap');

  .controls {
    text-align: center;
    margin-top: 2em;
    font-family: 'Orbitron', monospace;
  }

  .game-counter {
    color: #00ff41;
    font-size: 1.1rem;
    font-weight: 700;
    text-shadow: 0 0 10px #00ff41;
    margin-bottom: 1.5em;
    letter-spacing: 2px;
  }

  .instructions {
    color: #ff00ff;
    font-size: 0.9rem;
    margin-top: 1.5em;
    text-shadow: 0 0 8px #ff00ff;
    letter-spacing: 1px;
    opacity: 0.8;
  }

  .launch-btn {
    background: linear-gradient(145deg, #ff0066, #cc0044);
    color: white;
    border: 3px solid #ff00ff;
    padding: 1.2em 2.5em;
    font-size: 1.3rem;
    font-family: 'Orbitron', monospace;
    font-weight: 700;
    cursor: pointer;
    border-radius: 12px;
    margin: 1em 0;
    position: relative;
    overflow: hidden;
    text-transform: uppercase;
    letter-spacing: 2px;
    transition: all 0.3s ease;
    box-shadow: 
      0 0 20px #ff00ff,
      inset 0 0 20px rgba(255, 0, 255, 0.1);
  }

  .launch-btn::before {
    content: '';
    position: absolute;
    top: 0;
    left: -100%;
    width: 100%;
    height: 100%;
    background: linear-gradient(
      90deg,
      transparent,
      rgba(255, 255, 255, 0.3),
      transparent
    );
    transition: left 0.6s;
  }

  .launch-btn:hover {
    background: linear-gradient(145deg, #ff1177, #dd0055);
    transform: translateY(-2px);
    box-shadow: 
      0 0 30px #ff00ff,
      0 6px 20px rgba(255, 0, 102, 0.4),
      inset 0 0 30px rgba(255, 0, 255, 0.2);
  }

  .launch-btn:hover::before {
    left: 100%;
  }

  .launch-btn:active {
    transform: translateY(0);
    box-shadow: 
      0 0 15px #ff00ff,
      inset 0 0 15px rgba(255, 0, 255, 0.3);
  }

  .btn-text {
    position: relative;
    z-index: 1;
    text-shadow: 0 0 10px rgba(255, 255, 255, 0.8);
  }

  .btn-glow {
    position: absolute;
    top: 50%;
    left: 50%;
    width: 0;
    height: 0;
    background: radial-gradient(circle, rgba(255, 0, 255, 0.6) 0%, transparent 70%);
    border-radius: 50%;
    transform: translate(-50%, -50%);
    transition: all 0.3s ease;
    pointer-events: none;
  }

  .launch-btn:hover .btn-glow {
    width: 200px;
    height: 200px;
  }
</style>