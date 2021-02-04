/* eslint-disable no-prototype-builtins,no-restricted-syntax */
import 'whatwg-fetch';
import auth from '../common/auth';
import { API_ROOT } from '../configs/config';
import { SYMBOL_FOR_FETCH_API } from '../constants/fetchType';
import methodType from '../constants/methodType';

function isEnableBody(method) {
    return method !== methodType.GET;
}

export default function thunkMiddleware() {
    return (next) => (action) => {
        const API_OPTIONS = action[SYMBOL_FOR_FETCH_API];
        const fetchOptions = {};
        let queryString = '';

        if (typeof API_OPTIONS === 'undefined') {
            return next(action);
        }

        const { types, uri, method, body, onSuccess, onFail } = API_OPTIONS;
        const [requestType, successType, failureType] = types;

        next(
            Object.assign({}, action, {
                type: requestType,
                body,
            })
        );

        Object.assign(fetchOptions, {
            method,
            headers: {
                Accept: 'application/json',
                'Content-Type': 'application/json',
                Authorization: `Bearer ${auth.getAccessToken()}`,
            },
        });

        if (isEnableBody(method)) {
            fetchOptions.body = JSON.stringify(body);
        }

        if (!isEnableBody(method) && typeof body === 'object') {
            queryString = [];

            for (const key in body) {
                if (body.hasOwnProperty(key)) {
                    queryString.push(
                        `${key}=${encodeURIComponent(
                            typeof body[key] === 'object'
                                ? JSON.stringify(body[key])
                                : body[key]
                        )}`
                    );
                }
            }

            queryString = queryString.join('&');

            if (uri.indexOf('?') === -1) {
                queryString = `?${queryString}`;
            }
        }

        return fetch(API_ROOT + uri + queryString, fetchOptions)
            .then((response) =>
                response.text().then((text) => {
                    try {
                        return {
                            json: JSON.parse(text),
                            response,
                        };
                    } catch (e) {
                        return {
                            json: text,
                            response,
                        };
                    }
                })
            )
            .then(({ json, response }) => {
                if (!response.ok) {
                    return Promise.reject(json);
                }

                return json;
            })
            .then(
                (response) => {
                    const data = Object.assign({}, action, {
                        response,
                        type: successType,
                        body,
                    });

                    if (typeof onSuccess === 'function') {
                        onSuccess(data);
                    }

                    return next(data);
                },
                (error) => {
                    const data = Object.assign({}, action, {
                        type: failureType,
                        error:
                            typeof error === 'object'
                                ? error.message || 'Unknown Error'
                                : error,
                        body,
                    });

                    if (typeof onFail === 'function') {
                        onFail(data);
                    }

                    return next(data);
                }
            );
    };
}
