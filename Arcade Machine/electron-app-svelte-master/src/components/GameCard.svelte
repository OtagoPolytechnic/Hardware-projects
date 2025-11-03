<script>
  export let game;
  export let isActive = false;

  // Determine the image path based on the game object
  $: imgPath = game && game.image
    ? 'file:///' + game.image.replace(/\\/g, '/')
    : 'images/default-game.svg';
</script>

<button 
  class="game-card {isActive ? 'active' : ''}"
  on:click
  aria-label={`Launch ${game.name}`}>
  <div class="card-inner">
    <div class="image-container">
      <img src={imgPath} alt={`Image of ${game.name}`} />
      <div class="image-overlay"></div>
    </div>
    <h3 class="game-title">{game.name}</h3>
    g
  </div>
  <div class="card-glow"></div>
</button>

<style>
  @import url('https://fonts.googleapis.com/css2?family=Orbitron:wght@600&display=swap');

  .game-card {
    width: clamp(180px, 20vw, 220px);
    min-width: 180px;
    text-align: center;
    padding: 0;
    border: 3px solid transparent;
    border-radius: 16px;
    cursor: pointer;
    transition: all 0.4s ease;
    background: linear-gradient(145deg, #0a0015, #1a0033);
    color: #00ffff;
    position: relative;
    overflow: hidden;
    box-shadow: 
      0 4px 15px rgba(0, 0, 0, 0.3),
      inset 0 1px 0 rgba(255, 255, 255, 0.1);
  }

  .game-card::before {
    content: '';
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background: repeating-linear-gradient(
      45deg,
      transparent 0px,
      rgba(0, 255, 255, 0.02) 1px,
      transparent 2px
    );
    pointer-events: none;
  }

  .card-inner {
    padding: clamp(10px, 2vw, 20px);
    position: relative;
    z-index: 2;
  }

  .card-glow {
    position: absolute;
    top: -50%;
    left: -50%;
    width: 200%;
    height: 200%;
    background: radial-gradient(circle, rgba(0, 255, 255, 0.1) 0%, transparent 70%);
    opacity: 0;
    transition: opacity 0.4s ease;
    pointer-events: none;
  }

  .game-card.active {
    border-color: #ff00ff;
    background: linear-gradient(145deg, #330066, #4d0080);
    box-shadow: 
      0 0 25px #ff00ff,
      0 0 50px rgba(255, 0, 255, 0.3),
      inset 0 0 20px rgba(255, 0, 255, 0.1);
    transform: translateY(-5px) scale(1.02);
  }

  .game-card.active .card-glow {
    opacity: 1;
    background: radial-gradient(circle, rgba(255, 0, 255, 0.15) 0%, transparent 70%);
  }

  .game-card:hover:not(.active) {
    border-color: #00ffff;
    background: linear-gradient(145deg, #001a33, #003366);
    box-shadow: 
      0 0 20px #00ffff,
      0 8px 25px rgba(0, 255, 255, 0.2),
      inset 0 0 15px rgba(0, 255, 255, 0.05);
    transform: translateY(-3px) scale(1.01);
  }

  .game-card:hover .card-glow {
    opacity: 0.8;
  }

  .image-container {
    position: relative;
    margin-bottom: clamp(10px, 2vh, 15px);
  }

  .game-card img {
    width: clamp(120px, 15vw, 160px);
    height: clamp(120px, 15vw, 160px);
    object-fit: cover;
    border-radius: 12px;
    border: 2px solid rgba(0, 255, 255, 0.3);
    transition: all 0.3s ease;
    filter: brightness(0.9) contrast(1.1);
  }

  .image-overlay {
    position: absolute;
    top: 0;
    left: 50%;
    transform: translateX(-50%);
    width: clamp(120px, 15vw, 160px);
    height: clamp(120px, 15vw, 160px);
    border-radius: 12px;
    background: linear-gradient(
      45deg,
      rgba(0, 255, 255, 0.1) 0%,
      transparent 50%,
      rgba(255, 0, 255, 0.1) 100%
    );
    opacity: 0;
    transition: opacity 0.3s ease;
    pointer-events: none;
  }

  .game-card:hover .image-overlay,
  .game-card.active .image-overlay {
    opacity: 1;
  }

  .game-card.active img {
    border-color: rgba(255, 0, 255, 0.6);
    filter: brightness(1.1) contrast(1.2);
  }

  .game-card:hover img {
    border-color: rgba(0, 255, 255, 0.6);
    filter: brightness(1.05) contrast(1.15);
  }

  .game-title {
    margin: 0;
    font-size: clamp(0.9rem, 2vw, 1.3rem);
    font-family: 'Orbitron', monospace;
    font-weight: 600;
    text-transform: uppercase;
    letter-spacing: 2px;
    text-shadow: 0 0 10px currentColor;
    transition: all 0.3s ease;
    word-break: break-word;
  }

  .game-card.active .game-title {
    color: #ff00ff;
    text-shadow: 0 0 15px #ff00ff;
  }

  .game-card:hover .game-title {
    text-shadow: 0 0 12px currentColor;
  }

  @media (max-width: 768px) {
    .game-card {
      min-width: 150px;
    }
  }
</style>