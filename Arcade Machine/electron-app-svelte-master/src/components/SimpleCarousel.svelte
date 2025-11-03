<script>
  import { onMount } from 'svelte';
  import GameCard from './GameCard.svelte';
  import CarouselControls from './CarouselControls.svelte';

  export let games = [];
  
  let activeIndex = 0;
  let supportedRoms = {};
  
  // Load supported ROMs on mount
  onMount(async () => {
    try {
      supportedRoms = await window.electronAPI.getSupportedRoms();
    } catch (error) {
      console.error('Failed to load supported ROMs:', error);
    }
    
    const handleKeydown = (e) => {
      if (e.key === 'ArrowLeft' || e.key === 'a') {
        goToPrev();
      } else if (e.key === 'ArrowRight' || e.key === 'd') {
        goToNext();
      } else if (e.key === 'Enter' || e.key === ' ') {
        launchGame();
      }
    };
    window.addEventListener('keydown', handleKeydown);
    
    return () => {
      window.removeEventListener('keydown', handleKeydown);
    };
  });
  
  function goToNext() {
    if (games.length === 0) return;
    activeIndex = (activeIndex + 1) % games.length;
  }
  
  function goToPrev() {
    if (games.length === 0) return;
    activeIndex = (activeIndex - 1 + games.length) % games.length;
  }

  function launchGame() {
    const game = games[activeIndex];

    if (!game) {
      console.log('No game found');
      return;
    }

    // Get the file extension
    const ext = game.path.split('.').pop().toLowerCase();

    // looks at extension to check if rom
    const coreFileName = supportedRoms[ext];
    if (coreFileName) {
      window.electronAPI.runRetroarchRom(game.path, coreFileName);
    } else {
      window.electronAPI.openExePath(game.path);
    }
  }
</script>


<section class="carousel">
  <div class="carousel-border">
    <div class="carousel-inner">
      <section class="games-container">
        <div class="wheel-container" style="transform: translateX({-activeIndex * 280}px)">
          {#each games as game, index}
            <div 
              class="game-position" 
              style="
                transform: translateX({index * 280}px);
                z-index: {10 - Math.abs(index - activeIndex)};
                opacity: {index === activeIndex ? 1 : Math.max(0.3, 1 - Math.abs(index - activeIndex) * 0.25)};
                filter: {Math.abs(index - activeIndex) > 2 ? `blur(${Math.abs(index - activeIndex)}px)` : 'none'};
                scale: {index === activeIndex ? 1 : Math.max(0.7, 1 - Math.abs(index - activeIndex) * 0.1)};
              "
            >
              <GameCard
                {game}
                {index}
                isActive={index === activeIndex}
                on:click={() => activeIndex = index}
              />
            </div>
          {/each}
        </div>
      </section>
    </div>
  </div>
</section>

<CarouselControls 
  {activeIndex} 
  totalGames={games.length} 
  onLaunch={launchGame} 
/>

<style>
  .carousel {
    margin: clamp(1em, 3vh, 2em) auto;
    position: relative;
    max-width: 100%;
    overflow: hidden;
  }

  .carousel-border {
    background: linear-gradient(135deg, #0a0015 0%, #1a0033 50%, #0a0015 100%);
    border: clamp(2px, 0.5vw, 4px) solid #00ffff;
    border-radius: clamp(12px, 2vw, 20px);
    padding: clamp(4px, 1vw, 8px);
    position: relative;
    overflow: hidden;
    box-shadow: 
      0 0 30px #00ffff,
      0 0 60px rgba(0, 255, 255, 0.3),
      inset 0 0 30px rgba(0, 255, 255, 0.1);
  }

  .carousel-border::before {
    content: '';
    position: absolute;
    top: -2px;
    left: -2px;
    right: -2px;
    bottom: -2px;
    background: linear-gradient(
      45deg,
      #00ffff,
      #ff00ff,
      #ffff00,
      #00ffff
    );
    background-size: 400% 400%;
    border-radius: clamp(12px, 2vw, 20px);
    z-index: -1;
    animation: borderGlow 4s ease-in-out infinite;
  }

  .carousel-border::after {
    content: '';
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background: repeating-linear-gradient(
      90deg,
      transparent 0px,
      rgba(0, 255, 255, 0.03) 2px,
      transparent 4px,
      transparent 8px
    );
    pointer-events: none;
    border-radius: clamp(10px, 2vw, 16px);
  }

  .carousel-inner {
    background: linear-gradient(135deg, #1a0033 0%, #330066 30%, #1a0033 70%, #0d001a 100%);
    border-radius: clamp(10px, 2vw, 16px);
    position: relative;
    overflow: hidden;
  }

  .carousel-inner::before {
    content: '';
    position: absolute;
    top: 0;
    left: -100%;
    width: 100%;
    height: 100%;
    background: linear-gradient(
      90deg,
      transparent,
      rgba(255, 255, 255, 0.05),
      transparent
    );
    animation: sweep 6s infinite;
    pointer-events: none;
  }

  .games-container {
    display: flex;
    padding: clamp(15px, 3vw, 30px);
    flex: 1;
    justify-content: center;
    align-items: center;
    position: relative;
    height: clamp(280px, 40vh, 350px);
    overflow: hidden;
    /* Enhanced retro scanlines effect */
    background: 
      repeating-linear-gradient(
        to bottom,
        rgba(0, 255, 255, 0.02) 0px,
        rgba(0, 255, 255, 0.02) 1px,
        transparent 1px,
        transparent 4px
      ),
      repeating-linear-gradient(
        to right,
        rgba(255, 0, 255, 0.01) 0px,
        rgba(255, 0, 255, 0.01) 1px,
        transparent 1px,
        transparent 8px
      );
  }

  .games-container::after {
    content: '';
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background: radial-gradient(
      ellipse at center,
      transparent 40%,
      rgba(0, 0, 0, 0.1) 100%
    );
    pointer-events: none;
  }

  .wheel-container {
    position: relative;
    display: flex;
    align-items: center;
    justify-content: center;
    height: 100%;
    transition: transform 0.8s cubic-bezier(0.25, 0.46, 0.45, 0.94);
    left: 0;
  }

  .game-position {
    position: absolute;
    left: 50%;
    transform: translateX(-50%);
    transition: all 0.8s cubic-bezier(0.25, 0.46, 0.45, 0.94);
    transform-origin: center;
  }

  @keyframes borderGlow {
    0%, 100% {
      background-position: 0% 50%;
    }
    50% {
      background-position: 100% 50%;
    }
  }

  @keyframes sweep {
    0% { left: -100%; }
    100% { left: 100%; }
  }

  @media (max-width: 768px) {
    .carousel {
      margin: 1em 0;
    }

    .games-container {
      height: clamp(250px, 35vh, 300px);
      padding: 10px;
    }
  }

  @media (max-width: 480px) {
    .games-container {
      height: clamp(220px, 30vh, 270px);
    }
  }
</style>