import React from 'react';
import { Button, Tag, Image, Card, List, Row, Col, Dropdown, Menu } from 'antd';
import ReactCustomScrollBars from 'react-custom-scrollbars';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { send } from '../../middleware/websocket';
import { get, last, find } from 'lodash';
import { getIconPath } from '../../common/global';
import './tabs.less';
const { remote } = window.require('electron');

class FileView extends React.Component {
    isClicked = false;
    previews = {
        '.mp3': 'mp3.png',
        '.swf': 'swf.png',
        '.json': 'json.png',
        '.csb': 'csb.png',
        '.plist': 'plist.png',
    };

    tags = {
        '.png': <Tag color="#2db7f5">图片</Tag>,
        '.plist': <Tag color="#2db7f5">Plist</Tag>,
        '.mp3': <Tag color="#2db7f5">音效</Tag>,
        '.swf': <Tag color="#2db7f5">动画</Tag>,
        '.json': <Tag color="#2db7f5">配置</Tag>,
        '.csb': <Tag color="#2db7f5">CSB</Tag>,
    };

    getPreview = (item) => {
        console.log(item, "item")
        if (item.type === '.png') {
            return (
                <img
                    src={'file://' + item.path}
                    alt={'file://' + item.path}
                    style={{ height: 60, width: 'auto', maxWidth: '99%' }}
                />
            );
        } else {
            const preview = this.previews[item.type]
            return (
                preview && <img
                    src={getIconPath(preview)}
                    alt={item.path}
                    style={{ height: 60, width: 'auto', maxWidth: '99%' }}
                />
            );
        }
    };

    componentDidMount() {
        const { templateId, skinId, dispatch, unzipPath } = this.props;
        console.log('componentDidMount ');
        dispatch(
            send({
                id: 'files',
                template_id: templateId,
                skin_id: skinId,
                unzip_path: unzipPath,
            })
        );
    }

    // componentWillReceiveProps(nextProps) {
    //     const {templateId, skinId, dispatch} = nextProps;
    //     const {templateId : oldTemplateId, skinId : oldSkinId} = this.props;
    //    console.log("Files componentWillReceiveProps")
    //     if (templateId!=oldTemplateId || skinId!=oldSkinId){
    //         dispatch(send({
    //             "id": 'files',
    //             "template_id": templateId,
    //             "skin_id": skinId,
    //         }))
    //     }
    // }

    render() {
        const { dataWebsocket, dispatch, templateId, skinId, type } = this.props;
        const files = get(dataWebsocket, ['files', 'files', type], []);
        console.log(files)
        return (
                <ReactCustomScrollBars
                    autoHide
                    autoHideTimeout={600}
                    autoHideDuration={400}
                >
                    <List
                        grid={{
                            gutter: 16,
                            xs: 1,
                            sm: 2,
                            md: 4,
                            lg: 4,
                            xl: 6,
                            xxl: 3,
                        }}
                        dataSource={files}
                        renderItem={(item) => (
                            <List.Item>
                                <Card
                                    bodyStyle={{ padding: '2px 3px 0px' }}
                                    hoverable
                                    onClick={() => {
                                        dispatch(
                                            send({
                                                id: 'open_file',
                                                path: item.path,
                                            })
                                        );
                                    }}
                                >
                                    <Col style={{ width: '100%' }}>
                                        <Row align="center">
                                            {this.getPreview(item)}
                                        </Row>
                                        <Row align="center">
                                            <b
                                                style={{
                                                    wordBreak: 'break-all',
                                                    width: '100%',
                                                    textAlign: 'center',
                                                }}
                                            >
                                                {item.name}
                                            </b>
                                        </Row>
                                    </Col>
                                </Card>
                            </List.Item>
                        )}
                    />
                </ReactCustomScrollBars>
        );
    }
}

FileView.propTypes = {
    dispatch: PropTypes.func.isRequired,
    templateId: PropTypes.number.isRequired,
    skinId: PropTypes.number.isRequired,
    type: PropTypes.string.isRequired,
    unzipPath: PropTypes.string.isRequired,
};

function stateToProps(state) {
    const { dataWebsocket } = state;
    return { dataWebsocket };
}

export default connect(stateToProps)(FileView);
