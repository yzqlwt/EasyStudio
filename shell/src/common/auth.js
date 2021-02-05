import { AUTH_SERVER, REFRESH_SERVER } from '../configs/config';
import { setToken, getToken } from './global'

// 1000 * 60 * 5;
const THRESHOLD = 300000;
// 1000 * 60 * 2;
const REFRESH_INTERVAL = 120000 / 100;
const MILLISECOND = 1000;
const Secretary = 810;
const Editor = 800;
const CourseAdmin = 30;
const Admin = 20;

const IS_INTEGRATION = AUTH_SERVER.indexOf(':2443') > -1;

function toMillSecond(second) {
    return second * MILLISECOND;
}

function persistentToLocalStorage(token) {
    // cover max-age to expires.
    token.expires_in = toMillSecond(token.expires_in) + Date.now();
    setToken(JSON.stringify(token));
}

function mergeLocalStorage(token) {
    const origin = getToken();
    const newToken = {
        ...origin,
        ...token,
    };

    persistentToLocalStorage(newToken);
}

function extractJSON(response) {
    const contentType = response.headers.get('content-type');
    const isJSONResponse =
        contentType && contentType.indexOf('application/json') !== -1;
    const isResponseSuccess = response.ok;

    if (!isJSONResponse) {
        throw new Error('unknown error');
    }

    return response.json().then((json) => {
        if (!isResponseSuccess) {
            throw new Error(json.message);
        }

        return json;
    });
}

const auth = {
    refreshTokenTimer: null,

    login(mobile, password, authCode) {
        if (this.loggedIn()) {
            return Promise.resolve();
        }
        console.log(mobile);
        const headers = {
            Accept: 'application/json',
            'Content-Type': 'application/json',
        };

        return fetch(AUTH_SERVER, {
            mode: 'cors',
            method: 'POST',
            headers,
            body: JSON.stringify({
                mobile,
                password,
                nonce: authCode,
            }),
        })
            .then(extractJSON)
            .then(persistentToLocalStorage);
    },

    logout() {
        localStorage.removeItem(IS_INTEGRATION ? 'token_integration' : 'token');

        return Promise.resolve();
    },

    loggedIn() {
        // skip login if not in production mode.
        // if (process.js.env.NODE_ENV !== 'production') {
        //   return true;
        // }

        const token = this.deSerializeToken();
        console.log(token);
        return token && token.expires_in > Date.now();
    },

    getAccessToken() {
        const token = this.deSerializeToken();

        if (token) {
            return token.access_token;
        }

        return null;
    },

    deSerializeToken() {
        const token = getToken()
        return token;
    },

    initFreshTokenTimmer() {
        if (this.refreshTokenTimer) {
            clearTimeout(this.refreshTokenTimer);
        }

        this.refreshTokenTimer = setTimeout(
            this.refreshToken.bind(this),
            REFRESH_INTERVAL
        );
    },

    isValidToken() {
        const token = this.deSerializeToken();
        if (!token) {
            return -1;
        }
        if (token.expires_in > Date.now() + THRESHOLD) {
            return 1;
        } else {
            this.refreshToken();
            return 0;
        }
    },

    refreshToken() {
        this.initFreshTokenTimmer();

        const token = this.deSerializeToken();

        if (!token) {
            if (this.refreshTokenTimer) {
                clearTimeout(this.refreshTokenTimer);
            }
            return;
        }

        if (token.expires_in > Date.now() + THRESHOLD) {
            return Promise.resolve();
        }

        const headers = {
            Accept: 'application/json',
            'Content-Type': 'application/json',
        };

        return fetch(REFRESH_SERVER, {
            mode: 'cors',
            method: 'POST',
            headers,
            body: JSON.stringify({
                refresh_token: token.refresh_token,
            }),
        })
            .then(extractJSON)
            .then(mergeLocalStorage);
    },

    hasRole(role) {
        const token = this.deSerializeToken() || {};

        return token.roleId === role || this.hasScope(role, token.scopes);
    },

    hasScope(role, scopes = []) {
        const scopeMap = {
            [Admin]: 'admin',
            [CourseAdmin]: 'admin-course-admin',
            [Editor]: 'admin-course-editor',
            [Secretary]: 'admin-course-secretary',
        };

        return scopes.indexOf(scopeMap[role]) !== -1;
    },

    roleInPreDefined() {
        const roles = [Admin, Editor, Secretary, CourseAdmin];

        return roles.find((role) => this.hasRole(role));
    },

    isSecretary() {
        return this.hasRole(Secretary);
    },

    isAdmin() {
        return this.hasRole(Admin) || this.hasRole(CourseAdmin);
    },
    isEditor() {
        return this.hasRole(Editor);
    },
};

function ensureScope() {
    const token = auth.deSerializeToken();

    if (!token) {
        return;
    }
    if (!token.scopes) {
        // localStorage.clear();
        localStorage.removeItem(IS_INTEGRATION ? 'token_integration' : 'token');
        // location.reload();
    }
}

ensureScope();

export default auth;
