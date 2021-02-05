import React from 'react';
import { Drawer, Steps, Radio, Space } from 'antd';
import {
    UserOutlined,
    SolutionOutlined,
    LoadingOutlined,
    SmileOutlined,
} from '@ant-design/icons';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { send } from '../../middleware/websocket';
import { get, last, find, indexOf, map } from 'lodash';

const { Step } = Steps;

class StepsView extends React.Component {
    state = {
        visible: false,
    };

    showDrawer = () => {
        this.setState({
            visible: true,
        });
    };

    onClose = () => {
        this.setState({
            visible: false,
        });
    };

    onChange = (e) => {
        this.setState({
            placement: e.target.value,
        });
    };

    getSteps = () => {};

    componentWillReceiveProps(nextProps) {
        const { dataWebsocket } = nextProps;
        const { visible } = this.state;
        const steps = get(dataWebsocket, ["steps", "steps"], []);
        const current = get(dataWebsocket, 'steps.current', []);
        if (steps.length == 0) {
            return;
        }
        const lastStep = last(steps);
        if (visible != (lastStep !== current)) {
            this.setState({
                visible: lastStep !== current,
            });
        }
        if (lastStep !== current) {
            const curIndex = indexOf(steps, current);
            const tmp = steps.map((step, index) => {
                let status = 'wait';
                if (index < curIndex) {
                    status = 'finish';
                } else if (index == curIndex) {
                    status = 'process';
                }
                return {
                    text: step,
                    status: status,
                };
            });
            this.setState({
                steps: tmp,
            });
            console.log(tmp, curIndex);
        }
    }

    render() {
        const { visible, steps } = this.state;
        console.log(steps, 'STEPS');
        return (
            <Drawer
                title="Wait a minute"
                placement="bottom"
                closable={false}
                visible={visible}
                key="Drawer"
            >
                <Steps>
                    {map(steps, (step) => {
                        return <Step status={step.status} title={step.text} />;
                    })}
                </Steps>
            </Drawer>
        );
    }
}

// <Step status="finish" title="Login" icon={<UserOutlined />} />
// <Step status="finish" title="Verification" icon={<SolutionOutlined />} />
// <Step status="process" title="Pay" icon={<LoadingOutlined />} />
// <Step status="wait" title="Done" icon={<SmileOutlined />} />

StepsView.propTypes = {
    dispatch: PropTypes.func.isRequired,
    templateId: PropTypes.number.isRequired,
    skinId: PropTypes.number.isRequired,
};

function stateToProps(state) {
    const { dataWebsocket } = state;
    return { dataWebsocket };
}

export default connect(stateToProps)(StepsView);
