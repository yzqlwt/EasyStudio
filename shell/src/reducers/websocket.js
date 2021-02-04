import {
    WEBSOCKET_BROKEN,
    WEBSOCKET_CLOSED,
    WEBSOCKET_CONNECT,
    WEBSOCKET_DISCONNECT,
    WEBSOCKET_MESSAGE,
    WEBSOCKET_OPEN,
    WEBSOCKET_SEND,
    DEFAULT_PREFIX,
} from '../middleware/websocket';

let initState = {
    origin: '',
    timestamp: 0,
    connected: false,
}

export const dataWebsocket = function(state = initState, action = {}){
    switch (action.type) {
        case DEFAULT_PREFIX + '::' + WEBSOCKET_CONNECT:
            return {
                ...state,
                url: action.payload.url,
                connected: false,
            };
        case DEFAULT_PREFIX + '::' + WEBSOCKET_OPEN:
            return {
                ...state,
                connected: true,
            };
        case DEFAULT_PREFIX + '::' + WEBSOCKET_BROKEN:
        case DEFAULT_PREFIX + '::' + WEBSOCKET_CLOSED:
            return {
                ...state,
                connected: false,
            };
        case DEFAULT_PREFIX + '::' + WEBSOCKET_MESSAGE:
            const data = action.payload.message;
            const key = data.id;
            return Object.assign({}, state, {
                [key]: Object.assign({}, state[key], data)
            });
        default:
            return state;
    }
}
