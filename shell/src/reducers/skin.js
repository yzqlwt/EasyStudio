import { SKIN, DEFAULT_MANAGE_PREFIX } from '../constants/fetchType';
import readyState from '../constants/readyState';

function getKey(action, key = ''){
    let data = action;
    if (key) {
        data = data[key];
    }
    return `${data.templateType}_${data.templateId}`;
}

let initState = {
    readyState: readyState.INITIALIZE,
    response: {},
    error: '',
};

export const dataSkin = function (state = initState, action = {}) {
    let key = null;
    let MANAGE_PREFIX = DEFAULT_MANAGE_PREFIX;
    switch (action.type) {
        case SKIN[MANAGE_PREFIX]:
            key = getKey(action, 'body');
            return Object.assign({}, state, {
                [key]: Object.assign({}, state[key], {
                    readyState: readyState.LOADING,
                }),
            });
        case SKIN[`${MANAGE_PREFIX}_SUCCESS`]:
            key = getKey(action, 'body');
            return Object.assign({}, state, {
                [key]: Object.assign({}, state[key], {
                    readyState: readyState.SUCCESS,
                    response: action.response,
                }),
            });
        case SKIN[`${MANAGE_PREFIX}_FAIL`]:
            key = getKey(action, 'body');
            return Object.assign({}, state, {
                [key]: Object.assign({}, state[key], {
                    readyState: readyState.FAIL,
                    error: action.error,
                }),
            });
        default:
            return state;
    }
};

