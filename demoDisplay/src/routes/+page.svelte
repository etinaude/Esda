<!-- listen to websocket localhost:3000-->

<!-- {"flexVals":[4095,2461,4095,4095,4095],"confidence":0.8062659502029419,"pose":"Fist"}  -->
<script lang="ts">
  import { onMount, onDestroy } from "svelte";

  let poseMap = ["Normal", "Fist", "Spread", "Point"];

  let ws = new WebSocket("ws://localhost:3000");

  let flexVals: number[] = [];
  let confidence = 0;
  let pose = 0;
  let raw: any = {};
  let color = "#eee";

  onMount(() => {
    ws.onopen = () => {
      console.log("connected");
    };

    ws.onmessage = (event) => {
      console.log(event.data);
      raw = JSON.parse(event.data);
      flexVals = raw.flexVals;
      confidence = Math.round(raw.confidence * 10000) / 100;
      pose = raw.pose;

      if (pose === 0) {
        color = "#41d17b";
      } else {
        color = "#d14141";
      }
    };
  });

  onDestroy(() => {
    ws.close();
  });
</script>

<div class="container">
  <div class="data">
    <h2>Received Data</h2>
    <p><b>Flex Values:</b> {flexVals.join(", ")}</p>
    <p><b>Confidence:</b> {confidence}%</p>
    <p><b>Pose:</b> {pose} - {poseMap[pose]}</p>
  </div>

  <div class="square" style="background:{color}"></div>
</div>

<style lang="scss">
  .data {
    border: 1px solid black;
    border-radius: 10px;
    width: 500px;
    height: 500px;
    padding: 50px;
    box-sizing: border-box;
  }

  .square {
    width: 500px;
    height: 500px;
    background-color: #eee;
    border-radius: 10px;
  }

  .container {
    display: flex;
    justify-content: space-around;
    margin-top: 150px;
  }
</style>
