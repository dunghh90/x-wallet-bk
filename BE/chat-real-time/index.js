const express = require('express');
const path = require('path');
const http = require('http');
const {Server} = require('socket.io');
const chanel = require('./chat-service.js')

const app = express();
const server = http.createServer(app);
const io = new Server(server);


app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname + '/index.html'));
})


const serviceChanel = chanel();

io.on('connection', (socket) => {
    socket.on('create', (data) => {
        const chanel = serviceChanel.create(data)
        io.emit(data, chanel);
    })
    socket.on("join-chanel", (chanelID) => {
        const joinChanel = serviceChanel.exist(chanelID)
        socket.emit(chanelID, joinChanel)
        socket.on(ChanelID, (data) => {
            serviceChanel.addMessage(chanelID, data);
            socket.emit(chanelID, data)
        })        
    })

    
})



const PORT = 3040;
server.listen(PORT, () => {
    console.log('listening on port', PORT)
})







