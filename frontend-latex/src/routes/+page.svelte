<script lang="ts">
    import Header from './Header.svelte';

    let name = $state('PunPK');
    let status: 'success' | 'error' = $state('success');

    function ToggleEvent() {
        status = status === 'success' ? 'error' : 'success';
    }

    let fullName = $derived(name + " " + 'Desr')

</script>

<Header name={name} se_name='wes' />
<h2>{fullName}</h2>

<input type="text" bind:value={name} />

<p>Status: {status}</p>
<button onclick={ToggleEvent}>Toggle Status</button>