const mongoose = require('mongoose');


const time_managments = new mongoose.Schema({
    heatingtime:{
        required: true,
        type: Number
    },
    lightTestTime:{
      required: true,
        type: Number
    },
    sophisticatedComponentTime:{
      required: true,
      type: Number
    },
    delayTime:{
      required: true,
      type: Number
    }
  });

module.exports = mongoose.model('time_managments', time_managments);