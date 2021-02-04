import { ACTIVITY, DEFAULT_MANAGE_PREFIX, SYMBOL_FOR_FETCH_API } from '../constants/fetchType';
import { GET } from '../constants/methodType';
import readyState from '../constants/readyState';
import { trimStart } from 'lodash';

const ACTIVITY_URI = '/activityTemplates';

export const fetchActivity = function (searchValues) {
    const MANAGE_PREFIX = DEFAULT_MANAGE_PREFIX;
    return {
        [SYMBOL_FOR_FETCH_API]: {
            types: [
                ACTIVITY[MANAGE_PREFIX],
                ACTIVITY[`${MANAGE_PREFIX}_SUCCESS`],
                ACTIVITY[`${MANAGE_PREFIX}_FAIL`],
            ],
            uri: `/${trimStart(ACTIVITY_URI, '/')}`,
            method: 'get',
            body: {
                ...searchValues,
            },
            onSuccess: ()=> console.log("onSuccess"),
            onFail: ()=> console.log("onFail"),
        },
    };
};
