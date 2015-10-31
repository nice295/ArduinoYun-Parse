// Use Parse.Cloud.define to define as many cloud functions as you want.
// For example:
Parse.Cloud.define("hello", function(request, response) {
    console.log(request.params);
    response.success("Hello my world!");
});
 
Parse.Cloud.define("event", function(request, response) {
  console.log(request.params);
 
  Parse.Push.send({
    where: new Parse.Query(Parse.Installation),
    data: {
      alert: "Season tickets on sale until next week!"
    }
  }, {
    success: function() { response.success("OK"); },
    error: function() { response.error("NOK"); }
  });
});
