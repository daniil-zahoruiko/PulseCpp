function makeRequest()
{
    fetch("http://127.0.0.1:12345/example", {
        method: "POST",
        body: JSON.stringify({ test: "test" })
    });
}