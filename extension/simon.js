var messageCounter = 1
let simonSays = function(text) {
  chrome.runtime.sendNativeMessage(
    "alexen.app",
    {
      messageId: messageCounter++,
      content: text,
      timestamp: new Date().toISOString()
    },
    function(response){
      console.log(response);
  })
}
