import React from 'react';
import { connect } from 'react-redux';
import { Button, Result } from 'antd';
import { SyncOutlined } from '@ant-design/icons';
import { connect as wsconnect } from '../../middleware/websocket';
import auth from '../../common/auth';

import { withRouter } from 'react-router-dom';

class Index extends React.Component {
    state = {
        connection: 'wait',
    };

    componentDidMount() {
        this.repeatId = setInterval(this.restart, 2000);
    }

    componentWillReceiveProps(nextProps, nextContext) {
        const { dataWebsocket } = nextProps;
        if (dataWebsocket.connected) {
            clearInterval(this.repeatId);
            const value = auth.isValidToken();
            if (value == 1) {
                this.props.history.push('/activity');
            }
        }
    }

    connectServer = () => {
        const { dispatch } = this.props;
        dispatch(wsconnect('ws://127.0.0.1:8090'));
    };

    restart = () => {
        console.log('restart');
        this.startServer();
        this.connectServer();
    };

    startServer = () => {
        if (process.env.NODE_ENV === 'production' || true ) {
            const { remote } = window.require('electron');
            let RESOURCES_PATH = remote.getGlobal('shareObject').RESOURCES_PATH;
            const exec = window.require('child_process').execFile;
            let path = '/kernel/EasyStudio';
            path =
                window.process.platform === 'win32'
                    ? '/kernel/EasyStudio.exe'
                    : path;
            console.log(RESOURCES_PATH + '/assets' + path);
            exec(RESOURCES_PATH + '/assets' + path, (err, data) => {
                console.log(err);
                console.log(data.toString());
                this.connectServer();
            });
        } else {
            this.connectServer();
        }
    };

    render() {
        const { dataWebsocket, history } = this.props;
        console.log(dataWebsocket, history);
        let connection = dataWebsocket.connected ? 'success' : 'wait';
        if (connection == 'success') {
            return (
                <Result
                    status="success"
                    title="The kernel service is successfully connected"
                    extra={[
                        <Button
                            key="enter"
                            onClick={() => {
                                this.props.history.replace('/login');
                            }}
                        >
                            Enter
                        </Button>,
                    ]}
                />
            );
        } else {
            return (
                <Result
                    icon={<SyncOutlined spin />}
                    title="Kernel service is starting..."
                />
            );
        }
    }
}

function stateToProps(state) {
    const { dataWebsocket } = state;
    return {
        dataWebsocket,
    };
}

export default withRouter(connect(stateToProps)(Index));
