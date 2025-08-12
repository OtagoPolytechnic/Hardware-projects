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
  <img src={imgPath} alt={`Image of ${game.name}`} />
  <h3>{game.name}</h3>
</button>

<style>
  .game-card {
    min-width: 200px;
    text-align: center;
    padding: 20px;
    border: 2px solid transparent;
    border-radius: 10px;
    cursor: pointer;
    transition: all 0.3s ease;
    background: transparent;
    color: inherit;
  }

  .game-card.active {
    border-color: #ff6b35;
    background: rgba(255, 107, 53, 0.1);
  }

  .game-card:hover {
    border-color: #ff6b35;
    background: rgba(255, 107, 53, 0.05);
  }

  .game-card img {
    width: 150px;
    height: 150px;
    object-fit: cover;
    border-radius: 10px; /* Increased for better aesthetics */
    margin-bottom: 10px;
  }

  .game-card h3 {
    margin: 0;
    font-size: 1.2rem;
  }
</style>
