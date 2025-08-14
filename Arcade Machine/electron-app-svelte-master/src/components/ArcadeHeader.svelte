<script>
	let title = "ARCADE";
</script>

<header class="arcade-header">
	<h1 class="arcade-title">{title}</h1>
</header>

<style>
	.arcade-header {
		background: #333;
		border: 2px solid #ff6b35;
		border-radius: 10px;
		padding: 20px;
		margin-bottom: 30px;
		text-align: center;
	}

	.arcade-title {
		font-family: Arial, sans-serif;
		font-size: 2.5rem;
		font-weight: bold;
		color: #ff6b35;
		margin: 0;
	}

	@media (max-width: 768px) {
		.arcade-title {
			font-size: 2rem;
		}
		
		.arcade-header {
			padding: 15px;
			margin-bottom: 20px;
		}
	}
</style>
