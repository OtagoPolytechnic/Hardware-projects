<script>
  export let title = "ARCADE";

</script>

<header class="arcade-header">
  <h1 class="arcade-title">{title}</h1>
</header>

<style>
  @import url("https://fonts.googleapis.com/css2?family=Orbitron:wght@700;900&display=swap");

  .arcade-header {
    background: linear-gradient(145deg, #1a0033, #0d001a);
    border: 4px solid #00ff41;
    border-radius: 16px;
    padding: 24px;
    margin-bottom: 30px;
    text-align: center;
    position: relative;
    overflow: hidden;
    box-shadow:
      0 0 30px #00ff41,
      0 0 60px #00ff41,
      inset 0 0 20px rgba(0, 255, 65, 0.1);
  }

  .arcade-header::before {
    content: "";
    position: absolute;
    top: 0;
    left: -100%;
    width: 100%;
    height: 100%;
    background: linear-gradient(
      90deg,
      transparent,
      rgba(0, 255, 65, 0.2),
      transparent
    );
    animation: shine 3s infinite;
  }

  .arcade-header::after {
    content: "";
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background: repeating-linear-gradient(
      0deg,
      transparent 0px,
      rgba(0, 255, 65, 0.03) 2px,
      transparent 4px
    );
    pointer-events: none;
  }

  .arcade-title {
    font-family: "Orbitron", monospace;
    font-size: 3rem;
    font-weight: 900;
    color: #00ff41;
    margin: 0;
    text-shadow:
      0 0 10px #00ff41,
      0 0 20px #00ff41,
      0 0 40px #00ff41;
    letter-spacing: 4px;
    position: relative;
    z-index: 1;
    animation: glow 2s ease-in-out infinite alternate;
  }

  @keyframes glow {
    from {
      text-shadow:
        0 0 10px #00ff41,
        0 0 20px #00ff41,
        0 0 40px #00ff41;
    }
    to {
      text-shadow:
        0 0 15px #00ff41,
        0 0 30px #00ff41,
        0 0 50px #00ff41,
        0 0 70px #00ff41;
    }
  }

  @keyframes shine {
    0% {
      left: -100%;
    }
    100% {
      left: 100%;
    }
  }

  @media (max-width: 768px) {
    .arcade-title {
      font-size: 2.2rem;
      letter-spacing: 2px;
    }

    .arcade-header {
      padding: 18px;
      margin-bottom: 20px;
    }
  }
</style>
