<template>
  <div class='calibration page'>
    <div class='card'>
      <h2>Muscle Map Calibration</h2>
      <div class="charts">
        <emgChart
          v-for="(signal, index) in emgSignals"
          :key="'emg-' + index"
          :emgId="'emg-' + index"
          :emgSignal='signal'
          :title='titles[index]'
        ></emgChart>
      </div>
    </div>
    <div class='card'>
      <calibrator></calibrator>
    </div>

  </div>
</template>

<script>
// import axios from 'axios'

import myFooter from './myFooter.vue'
import emgChart from './emgChart.vue'
import calibrator from './calibrator.vue'

const Fs = 300 // the amount of samples in ten seconds (streamed slow enough so the graph can draw it)

export default {
  data () {
    return {
      emgSignals: [
        Array.from(new Array(Fs), (val, i) => { return {x: -Fs + i, y: null} }),
        Array.from(new Array(Fs), (val, i) => { return {x: -Fs + i, y: null} }),
        Array.from(new Array(Fs), (val, i) => { return {x: -Fs + i, y: null} }),
        Array.from(new Array(Fs), (val, i) => { return {x: -Fs + i, y: null} }),
        Array.from(new Array(Fs), (val, i) => { return {x: -Fs + i, y: null} }),
        Array.from(new Array(Fs), (val, i) => { return {x: -Fs + i, y: null} }),
        Array.from(new Array(Fs), (val, i) => { return {x: -Fs + i, y: null} }),
        Array.from(new Array(Fs), (val, i) => { return {x: -Fs + i, y: null} })],

      titles: [
        'Electrode 1',
        'Electrode 2',
        'Electrode 3',
        'Electrode 4',
        'Electrode 5',
        'Electrode 6',
        'Electrode 7',
        'Electrode 8',
      ]
    }
  },
  components: {
    myFooter,
    emgChart,
    calibrator
  },
  sockets: {
    receiveSample (data) {
      for (var i = 0; i < 8; i++) {
        let newData = {x: this.t, y: data[i][0]}
        this.emgSignals[i].push(newData)
        this.emgSignals[i].shift()
      }
      this.t += 1
      this.samplesReceived += 1
    },
    endCalibration (data) {
      this.$router.push('/')
    }
  },
  created () {
    this.startCalibration()
  },
  mounted () {
    this.startSampleRequest()
    this.startCheckStatus()
    this.samplesReceived = 0
  },
  beforeDestroy () {
    this.stopSampleRequest()
    this.stopCheckStatus()
  },
  methods: {
    startCalibration () {
      this.$socket.emit('startCalibration')
    },
    requestSample () {
      this.$socket.emit('getSample')
    },
    calculateFreq () {
      console.log('Broadcast rate: '+(this.samplesReceived)+' samples per second.')
      this.samplesReceived = 0
    },
    startSampleRequest () {
      this.requestTimer = setInterval(this.requestSample, 32)
      this.statsTimer = setInterval(this.calculateFreq, 1000)
      this.t = 0
    },
    startCheckStatus () {
      this.checkStatus = setInterval(() => {
        this.$socket.emit('processingStatus')
      }, 32)
    },
    stopCheckStatus () {
      clearInterval(this.checkStatus)
    },
    stopSampleRequest () {
      clearInterval(this.requestTimer)
      // clearInterval(this.statsTimer)
      this.$socket.emit('stopCalib') // TODO: make this functional
    }
  }
}
</script>

<style lang='scss'>
@import '../styles/elements';
@import "../styles/colors";

.calibration {
  // height: 100vh;
  position: relative;
  display: flex;
  flex-flow: column;
  padding: 20px;

  h2 {
    margin-bottom: 20px;
  }

  .card:first-of-type {
    margin-top: 0;
  }

  .card:last-of-type {
    flex: 1 0 auto;
    display: flex;
    border-bottom-left-radius: 0;
    border-bottom-right-radius: 0;
  }
}
.charts {
  display: flex;
  flex-flow: row wrap;
  justify-content: space-between;
}
</style>
