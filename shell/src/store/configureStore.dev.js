import { createStore, applyMiddleware, compose, combineReducers } from 'redux';
import { routerReducer as routing } from 'react-router-redux';
import thunk from 'redux-thunk';
import { createLogger } from 'redux-logger';
import baseApi from '../middleware/baseApi';
import qkidGate from '../middleware/qkidGate';
import websocket, { DEFAULT_PREFIX } from '../middleware/websocket';
import reducers from '../reducers';
import propertyType from '../constants/propertyType';

const websocketMiddleware = websocket({
    prefix: DEFAULT_PREFIX,
    onOpen: (socket) => {
        // @ts-ignore
        window.__socket = socket; // eslint-disable-line no-underscore-dangle
    },
    dateSerializer: (date) => date.toISOString(),
    deserializer: JSON.parse,
});

const configureStore = (preloadedState) => {
    const store = createStore(
        combineReducers({
            ...reducers,
            routing,
        }),
        preloadedState,
        compose(
            applyMiddleware(
                thunk,
                baseApi,
                qkidGate,
                websocketMiddleware,
                createLogger()
            )
        )
    );
    return store;
};

export default configureStore;
