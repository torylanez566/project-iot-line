const express = require('express'); 
const mongoose = require('mongoose');

const app = express(); 
const port = 8000; 

app.use(express.static('public')); 
app.use(express.urlencoded({ extended: true })); 



const mongodbURL = 'mongodb+srv://yosefdego:Yd5668854@yosefdego12.sbzfv5z.mongodb.net/test'; // כתובת ה-URL של שרת MongoDB


mongoose.connect(mongodbURL);
const database = mongoose.connection

database.on('error', (error) => {
    console.log(error)
})

database.once('connected', () => {
    console.log('Database Connected');
})

const time_managment = require("./models/time_managment_model");
    

    app.get('/', (req, res) => {
      res.sendFile(__dirname + '/index.html');
    });

    

 
 app.post('/api/update', (req, res) => {
    console.log(req.body.step1)
  const modelData = new time_managment ({
    heatingtime:req.body.step1,
    lightTestTime:req.body.step2,
    sophisticatedComponentTime:req.body.step3,
    delayTime:req.body.step4
  });
  modelData.save();
  res.send('Saved');

});

app.get('/getleveltime/:prossec', async (req, res) => {
  try {
    const time_managment_model = await time_managment.findOne().sort({ _id: -1 }).limit(1);
    const formattedData = time_managment_model.toObject();
    console.log(req.params.prossec);
    res.json({ data: formattedData[req.params.prossec] });
  } catch (error) {
    console.error('Error retrieving data:', error);
    res.status(500).json({ error: 'Error retrieving data' });
  }
});



     // התחלת השרת
     app.listen(port, () => {
      console.log(`Server listening on port ${port}`);
    });