import { trimStart } from 'lodash';
import {
    SYMBOL_FOR_FETCH_API,
    DEFAULT_MANAGE_PREFIX,
} from '../constants/fetchType';

export function generateFetchData(types, uri, options = {}) {
    const MANAGE_PREFIX = DEFAULT_MANAGE_PREFIX;

    return function (searchValues) {
        return {
            [SYMBOL_FOR_FETCH_API]: {
                types: [
                    types[MANAGE_PREFIX],
                    types[`${MANAGE_PREFIX}_SUCCESS`],
                    types[`${MANAGE_PREFIX}_FAIL`],
                ],
                uri: `/${trimStart(uri, '/')}`,
                method: 'get',
                body: {
                    ...searchValues,
                },
                onSuccess: options.onSuccess,
                onFail: options.onFail,
            },
        };
    };
}
