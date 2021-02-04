/* eslint-disable no-prototype-builtins,no-restricted-syntax */
import readyState from '../constants/readyState';
// import imageNotFound from '../../../static/images/assets/image_not_found.png';

const STATIC_ROOT = 'http://gate-static.97kid.com';

function filterReadyState(currentReadyState) {
    if (!currentReadyState) {
        return readyState.INITIALIZE;
    }

    if (typeof currentReadyState === 'object') {
        return currentReadyState.readyState;
    }

    return currentReadyState;
}

export function isFetchInitialize(currentReadyState) {
    return filterReadyState(currentReadyState) === readyState.INITIALIZE;
}

export function isFetchLoading(currentReadyState) {
    return filterReadyState(currentReadyState) === readyState.LOADING;
}

export function isFetchSuccess(currentReadyState) {
    return filterReadyState(currentReadyState) === readyState.SUCCESS;
}

export function isFetchFail(currentReadyState) {
    return filterReadyState(currentReadyState) === readyState.FAIL;
}

export function startToPage(start, count) {
    return Math.ceil(start / count) + 1;
}

export function pageToStart(page, count) {
    return parseInt(((page || 1) - 1) * (count || 10), 10);
}

export function getAbsoluteStaticUrl(uri) {
    if (!uri) {
        return null;
    }

    if (/^https?:/i.test(uri)) {
        return uri;
    }

    return STATIC_ROOT + uri;
}
