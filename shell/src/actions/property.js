import { PROPERTY, DEFAULT_MANAGE_PREFIX, SYMBOL_FOR_FETCH_API } from '../constants/fetchType';
import { GET } from '../constants/methodType';
import { trimStart } from 'lodash';

const PROPERTY_URI = '/properties';

export const fetchProperty = function (searchValues) {
    return {
        [SYMBOL_FOR_FETCH_API]: {
            types: [
                PROPERTY[DEFAULT_MANAGE_PREFIX],
                PROPERTY[`${DEFAULT_MANAGE_PREFIX}_SUCCESS`],
                PROPERTY[`${DEFAULT_MANAGE_PREFIX}_FAIL`],
            ],
            uri: `/${trimStart(PROPERTY_URI, '/')}`,
            method: 'get',
            body: {
                ...searchValues,
            },
            onSuccess: ()=> console.log("onSuccess"),
            onFail: ()=> console.log("onFail"),
        },
    };
};
