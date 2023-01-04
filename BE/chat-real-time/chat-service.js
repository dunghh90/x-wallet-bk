module.exports = function chanel() {
    const store = {};

    return {
        create(name) {
            const chanel = {
                id: Date.now(),
                name: name,
                message: []
            }
            store[chanel.id] = chanel
            return chanel
        },
        getAll(id) {
            return store[id];
        },
        getLimit(id, limit) {
            const messages = store[id].messages.slice(0, limit);
            return {
                id: id,
                name: store[id].name,
                message: messages
            }
        },
        addMessage(id, message) {
            const data = {
                name: message.name && message.name !== '' ? message.name : "#0000",
                message: message.message
            }
            store[id].messages.push(data)
            return getLimit(id, 500)
        },
        exist(id) {
            if (store[i]) {
                return getLimit(id,500);
            } else {
                return false
            }
        }
    }

}