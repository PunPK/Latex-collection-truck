<script lang="ts">
  let leftWheelActive: boolean = false;
  let rightWheelActive: boolean = false;

  type Direction = 'NW' | 'N' | 'NE' | 'W' | 'E' | 'SW' | 'S' | 'SE' | null;
  let currentDirection: Direction = null;
  
  function toggleLeftWheel(): void {
    leftWheelActive = !leftWheelActive;
  }

  function toggleRightWheel(): void {
    rightWheelActive = !rightWheelActive;
  }

  function setDirection(dir: Direction): void {
    // If the user clicks the currently active direction, it turns it off (null).
    // Otherwise, it switches to the new direction.
    currentDirection = currentDirection === dir ? null : dir;
  }

  import car_body from '$lib/assets/car_body.png';
  import Header from './Header.svelte';
</script>

<div class="dashboard-container">
  <Header />

  <div class="dashboard-body">

    <div class="panel left-panel">
      <div class="car-layout">
        <button 
          class="status-indicator {leftWheelActive ? 'active' : 'inactive'}" 
          on:click={toggleLeftWheel}
          aria-label="Toggle Left Wheel"
        >
          L
        </button>

        <div class="car-body">
          <img src={car_body} alt="Car Body" width="120" height="180" />
        </div>

        <!-- Right Wheel Indicator -->
        <button 
          class="status-indicator {rightWheelActive ? 'active' : 'inactive'}" 
          on:click={toggleRightWheel}
          aria-label="Toggle Right Wheel"
        >
          R
        </button>
      </div>
      
      <div class="status-text">
        <p>Left Wheel: <strong>{leftWheelActive ? 'Active' : 'Idle'}</strong></p>
        <p>Right Wheel: <strong>{rightWheelActive ? 'Active' : 'Idle'}</strong></p>
      </div>
    </div>

    <!-- DIVIDER -->
    <div class="divider"></div>

    <!-- RIGHT PANEL: Directional Controller -->
    <div class="panel right-panel">
      <div class="d-pad">
        <!-- Top Row -->
        <button class="dir-btn {currentDirection === 'NW' ? 'active' : 'inactive'}" on:click={() => setDirection('NW')}>↖</button>
        <button class="dir-btn {currentDirection === 'N' ? 'active' : 'inactive'}" on:click={() => setDirection('N')}>↑</button>
        <button class="dir-btn {currentDirection === 'NE' ? 'active' : 'inactive'}" on:click={() => setDirection('NE')}>↗</button>

        <!-- Middle Row -->
        <button class="dir-btn {currentDirection === 'W' ? 'active' : 'inactive'}" on:click={() => setDirection('W')}>←</button>
        <div class="center-node">
          <span>Direction</span>
        </div>
        <button class="dir-btn {currentDirection === 'E' ? 'active' : 'inactive'}" on:click={() => setDirection('E')}>→</button>

        <!-- Bottom Row -->
        <button class="dir-btn {currentDirection === 'SW' ? 'active' : 'inactive'}" on:click={() => setDirection('SW')}>↙</button>
        <button class="dir-btn {currentDirection === 'S' ? 'active' : 'inactive'}" on:click={() => setDirection('S')}>↓</button>
        <button class="dir-btn {currentDirection === 'SE' ? 'active' : 'inactive'}" on:click={() => setDirection('SE')}>↘</button>
      </div>
      
      <div class="status-text">
        <p>Current Vector: <strong>{currentDirection || 'Neutral'}</strong></p>
      </div>
    </div>

  </div>
</div>

<style>
  :global(body) {
    font-family: 'Inter', system-ui, -apple-system, sans-serif;
    background-color: #f0f2f5;
    margin: 0;
    padding: 20px;
    display: flex;
    justify-content: center;
  }

  .dashboard-container {
    background: #f1f0f0;
    width: 100%;
    max-width: 900px;
    border-radius: 12px;
    box-shadow: 0 10px 30px rgba(0, 0, 0, 0.08);
    overflow: hidden;
  }
  .dashboard-body {
    display: flex;
    padding: 40px 20px;
    align-items: stretch;
  }

  .panel {
    flex: 1;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
  }

  .divider {
    width: 2px;
    background-color: #111;
    margin: 0 20px;
    border-radius: 2px;
  }

  .status-text {
    margin-top: 30px;
    text-align: center;
    font-size: 14px;
    color: #555;
  }

  /* --- LEFT PANEL: CAR STYLES --- */
  .car-layout {
    display: flex;
    align-items: center;
    gap: 20px;
  }

  .car-body {
    width: 120px;
    height: 180px;
    background-color: #ea4335; /* Red body matching reference */
    border-radius: 8px;
    display: flex;
    justify-content: center;
    align-items: center;
    box-shadow: inset 0 0 10px rgba(0,0,0,0.1);
  }

  .car-label {
    color: white;
    font-weight: 500;
  }

  .status-indicator {
    width: 40px;
    height: 80px;
    border-radius: 20px;
    border: none;
    cursor: pointer;
    font-weight: bold;
    transition: all 0.2s ease-in-out;
  }

  /* --- RIGHT PANEL: DIRECTIONAL STYLES --- */
  .d-pad {
    display: grid;
    grid-template-columns: repeat(3, 60px);
    grid-template-rows: repeat(3, 60px);
    gap: 15px;
    align-items: center;
    justify-items: center;
  }

  .dir-btn {
    width: 50px;
    height: 50px;
    border-radius: 50%;
    border: none;
    font-size: 20px;
    cursor: pointer;
    display: flex;
    justify-content: center;
    align-items: center;
    transition: all 0.2s ease-in-out;
  }

  .center-node {
    width: 60px;
    height: 60px;
    background-color: #d7bdeb; /* Light purple matching reference */
    border-radius: 50%;
    display: flex;
    justify-content: center;
    align-items: center;
    text-align: center;
    font-size: 10px;
    color: #333;
    font-weight: 600;
    box-shadow: inset 0 2px 4px rgba(0,0,0,0.1);
  }

  /* --- DYNAMIC STATE STYLES (Your color requirements) --- */
  .inactive {
    background-color: #d3d3d3
    color: #666;
  }

  .active {
    background-color: #1a1a1a;
    color: #ffffff;
    transform: scale(1.05);
    box-shadow: 0 4px 12px rgba(0, 0, 0, 0.3);
  }

  /* Hover effects for better UX */
  button.inactive:hover {
    background-color: #c0c0c0;
  }
</style>