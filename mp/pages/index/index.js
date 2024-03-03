const app = getApp()
const mqtt = require('../../utils/mqtt.min')

const options = {
  connectTimeout: 4000,
  clientId: 'wx_mp',
  port: 8084,
  username: 'rodney',
  password: '912267428'
}
const mqttHost = 'broker-cn.emqx.io'
const mqttPort = '8084'

const deviceSubTopic = '/smarthome_yk/sub' // 设备订阅  小程序发布命令的topic
const devicePubTopic = '/smarthome_yk/pub' // 设备发布  小程序订阅数据的topic
const mpSubTopic = devicePubTopic
const mpPubTopic = deviceSubTopic

Page({
  data: {
    client: null,
    Temp: 0,
    Hum: 0,
    Light: 0,
    LED: false,
    Buzzer: false
  },
  onLedchage: function (event) {
    const that = this
    const sw = event.detail.value

    that.setData({
      LED: sw
    })
    if (sw) {
      that.data.client.publish(mpPubTopic, JSON.stringify({
        target: "LED",
        value: 1
      }), function (err) {
        if (!err) {
          console.log("成功下发指令：开灯");
        }
      })
    } else {
      that.data.client.publish(mpPubTopic, JSON.stringify({
        target: "LED",
        value: 0
      }), function (err) {
        if (!err) {
          console.log("成功下发指令：关灯");
        }
      })
    }
  },

  onBuzzerchage: function (event) {
    const that = this
    const sw = event.detail.value

    that.setData({
      Buzzer: sw
    })
    if (sw) {
      that.data.client.publish(mpPubTopic, JSON.stringify({
        target: "Buzzer",
        value: 1
      }), function (err) {
        if (!err) {
          console.log("成功下发指令：开始报警");
        }
      })
    } else {
      that.data.client.publish(mpPubTopic, JSON.stringify({
        target: "Buzzer",
        value: 0
      }), function (err) {
        if (!err) {
          console.log("成功下发指令：结束报警");
        }
      })
    }
  },

  onLoad: function () {
    var that = this
    that.setData({
      client: mqtt.connect(`wxs://${mqttHost}:${mqttPort}/mqtt`, options)
    })
    that.data.client.on('connect', function (params) {
      console.log('成功连接到mqtt服务器')
      wx.showToast({
        title: '连接成功',
        mask: true,
        icon: 'success'
      })
    })

    that.data.client.subscribe(mpSubTopic, function (err) {
      if (!err) {
        console.log('成功订阅设备上行数据Topic！')
      }
    })

    that.data.client.on('message', function (topic, message) {
      //message 是16进制的buffer字节流
      let dataFromDev = {}
      try {
        dataFromDev = JSON.parse(message)
        // console.log(dataFromDev);
        that.setData({
          Temp: dataFromDev.Temp,
          Hum: dataFromDev.Hum,
          Light: dataFromDev.Light,
          LED: dataFromDev.LED,
          Buzzer: dataFromDev.Buzzer,
        })
      } catch (error) {
        // console.log(error)
        console.log('JSON解析失败', error)
      }

    })

  },
})