<script lang="ts">
    let { name, se_name = 'wes' } : {name : string; se_name? : string;} = $props();
</script>

<h1>Header Hello {name}</h1>
<h2>{name.replaceAll('K', '-')}</h2>

{se_name}

<style>


</style>