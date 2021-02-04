import { SKIN, DEFAULT_MANAGE_PREFIX, SYMBOL_FOR_FETCH_API } from '../constants/fetchType';
import { GET } from '../constants/methodType';
import { trimStart } from 'lodash';

const SKIN_URI = '/skin';

export const fetchSkin = function (searchValues) {
    return {
        [SYMBOL_FOR_FETCH_API]: {
            types: [
                SKIN[DEFAULT_MANAGE_PREFIX],
                SKIN[`${DEFAULT_MANAGE_PREFIX}_SUCCESS`],
                SKIN[`${DEFAULT_MANAGE_PREFIX}_FAIL`],
            ],
            uri: `/${trimStart(SKIN_URI, '/')}`,
            method: 'get',
            body: {
                ...searchValues,
            },
            onSuccess: ()=> console.log("onSuccess"),
            onFail: ()=> console.log("onFail"),
        },
    };
};
