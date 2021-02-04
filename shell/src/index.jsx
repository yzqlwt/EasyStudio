import React from 'react';
import { render } from 'react-dom';
import { Provider } from 'react-redux';
import { HashRouter as Router, Switch, Route } from 'react-router-dom';
import 'antd/dist/antd.css';
import ConnectServerRoute from './components/connect_server/Index';
import ActivityTemplateRoute from './components/activity_template/Index';
import LoginRoute from './components/login/Index';

import configureStore from './store/configureStore';

const store = configureStore();
render(
    <Provider store={store}>
        <Router>
            <Switch>
                <Route exact path="/" component={ConnectServerRoute} />
                <Route exact path="/connect" component={ConnectServerRoute} />
                <Route exact path="/login" component={LoginRoute} />
                <Route
                    exact
                    path="/activity"
                    component={ActivityTemplateRoute}
                />
            </Switch>
        </Router>
    </Provider>,
    document.getElementById('root')
);
