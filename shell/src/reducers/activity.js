import { ACTIVITY, DEFAULT_MANAGE_PREFIX} from '../constants/fetchType';
import readyState from '../constants/readyState';

let initState = {
    readyState: readyState.INITIALIZE,
    response: {},
    error: '',
};

export const dataActivity = function (state = initState, action = {}) {
    let MANAGE_PREFIX = DEFAULT_MANAGE_PREFIX;
    switch (action.type) {
        case ACTIVITY[MANAGE_PREFIX]:
            return Object.assign({}, state, {
                readyState: readyState.LOADING,
            });
        case ACTIVITY[`${MANAGE_PREFIX}_SUCCESS`]:
            return Object.assign({}, state, {
                readyState: readyState.SUCCESS,
                response: action.response,
            });
        case ACTIVITY[`${MANAGE_PREFIX}_FAIL`]:
            return Object.assign({}, state, {
                readyState: readyState.FAIL,
                error: action.error,
                response: [],
            });
        default:
            return state;
    }
};
