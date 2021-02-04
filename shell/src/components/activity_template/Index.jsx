import React from 'react';
import {
    message,
    Menu,
    Spin,
    Pagination,
    Tooltip,
    PageHeader,
    Button,
    Descriptions,
    Layout,
    Drawer,
    Col,
    Card,
    Divider,
    Dropdown,
} from 'antd';
import { get, last, find } from 'lodash';
import ReactCustomScrollBars from 'react-custom-scrollbars';
import PropTypes from 'prop-types';
import { withRouter } from 'react-router-dom';
import { connect } from 'react-redux';
import Toolbar from './Toolbar';
import StepsView from './StepsView';
import FileView from './FileView';
import { fetchActivity } from '../../actions/activity';
import { fetchSkin } from '../../actions/skin';
import { fetchProperty } from '../../actions/property';
import {
    isFetchInitialize,
    isFetchLoading,
    isFetchFail,
    isFetchSuccess,
    startToPage,
    pageToStart,
    getAbsoluteStaticUrl,
} from '../../common/library';
import { send } from '../../middleware/websocket';
import { EllipsisOutlined } from '@ant-design/icons';

class Index extends React.Component {
    state = {
        templateId: null,
        skinId: null,
        params: {
            count: 12,
        },
    };

    fetchActivity = () => {
        const { dispatch } = this.props;
        dispatch(fetchActivity(this.state.params));
    };

    fetchSkins = (templateId) => {
        const { dispatch } = this.props;
        dispatch(
            fetchSkin({
                templateId: templateId,
                templateType: 'ActivityTemplate',
            })
        );
    };

    fetchProperties = (skinId) => {
        const { dispatch } = this.props;
        console.log('fetchProperties');
        dispatch(fetchProperty({ itemId: skinId, itemType: 'Skin' }));
    };

    handlePageChange = (page) => {
        const { params } = this.state;
        params.start = pageToStart(page, params.count);
        this.setState(
            {
                params,
                templateId: null,
            },
            this.fetchActivity
        );
    };

    handleSearch = (searchFields) => {
        const { params } = this.state;

        searchFields.start = 0;
        this.setState(
            {
                params: { ...params, ...searchFields },
            },
            this.fetchActivity
        );
    };

    onOpenChange = (keys) => {
        const lastKey = last(keys);
        let templateId = parseInt(lastKey);
        templateId = isNaN(templateId) ? null : templateId;
        let state = {};
        if (templateId) {
            state = {
                templateId: templateId,
                skidId: null,
            };
            this.fetchSkins(templateId);
        } else {
            state = {
                templateId: null,
            };
        }
        this.setState(state);
    };

    handleClick = (e) => {
        let skinId = parseInt(e.key);
        skinId = isNaN(skinId) ? null : skinId;
        this.setState({
            skinId: skinId,
        });
        if (skinId) {
            this.fetchProperties(skinId);
        }
    };

    componentDidMount() {
        this.fetchActivity();
        const { dataWebsocket, history } = this.props;
        if (!dataWebsocket.connected) {
            history.replace('/connect');
        }
    }

    componentWillReceiveProps(nextProps, nextContext) {
        const { dataWebsocket, history } = nextProps;
        if (!dataWebsocket.connected) {
            history.replace('/');
        }
    }

    getSkinsData() {
        const { dataSkin } = this.props;
        const templateType = 'ActivityTemplate';
        const templateId = this.state.templateId;
        const targetSkin = get(dataSkin, templateType + '_' + templateId);
        if (targetSkin) {
            return get(targetSkin, 'response', []);
        }
        return [];
    }

    getTargetSkinData() {
        const skins = this.getSkinsData();
        const data = find(skins, (skin) => skin.id == this.state.skinId);
        return data;
    }

    getPath() {
        const { dataProperty } = this.props;
        const itemType = 'Skin';
        const itemId = this.state.skinId;
        const targetProperty = get(dataProperty, itemType + '_' + itemId);
        if (targetProperty) {
            const property = find(
                get(targetProperty, 'response', []),
                (property) => property.name == 'path'
            );
            if (property) {
                return property.content;
            }
        }
        return '未设置Path';
    }
    handleOpenFolder = (e) => {
        const { dispatch } = this.props;
        dispatch(
            send({
                id: 'go_to_folder',
                template_id: this.state.templateId,
                skin_id: this.state.skinId,
                type: e.key,
                unzip_path: this.getPath(),
            })
        );
    };

    menu = (
        <Menu>
            <Menu.Item key="images" onClick={this.handleOpenFolder}>
                csd
            </Menu.Item>
            <Menu.Item key="others" onClick={this.handleOpenFolder}>
                音效动画
            </Menu.Item>
            <Menu.Item key="cache" onClick={this.handleOpenFolder}>
                mango cache
            </Menu.Item>
        </Menu>
    );

    renderHeader() {
        const { dispatch } = this.props;
        const { templateId, skidId } = this.state;

        const data = this.getTargetSkinData();
        if (!data) {
            return <div></div>;
        }
        return (
            <PageHeader
                ghost={false}
                title={data.name || 'undefined'}
                subTitle=""
                extra={[
                    <Button
                        key="creator"
                        onClick={() => {
                            message.info('还没做完。。。');
                        }}
                    >
                        在Creator中打开
                    </Button>,
                    <Button
                        key="download"
                        onClick={() => {
                            dispatch(
                                send({
                                    id: 'download',
                                    template_id: this.state.templateId,
                                    skin_id: this.state.skinId,
                                    unzip_path: this.getPath(),
                                })
                            );
                        }}
                    >
                        下载
                    </Button>,
                    <Button
                        key="1"
                        type="primary"
                        onClick={() => {
                            dispatch(
                                send({
                                    id: 'upload',
                                    template_id: this.state.templateId,
                                    skin_id: this.state.skinId,
                                    unzip_path: this.getPath(),
                                })
                            );
                        }}
                    >
                        打包&上传
                    </Button>,
                    <Dropdown key="more" overlay={this.menu}>
                        <Button
                            style={{
                                border: 'none',
                                padding: 0,
                            }}
                        >
                            <EllipsisOutlined
                                style={{
                                    fontSize: 20,
                                    verticalAlign: 'top',
                                }}
                            />
                        </Button>
                    </Dropdown>,
                ]}
                style={{ margin: 0 }}
            >
                <Descriptions size="middle">
                    <Descriptions.Item label="解压路径：">
                        {this.getPath()}
                    </Descriptions.Item>
                </Descriptions>
            </PageHeader>
        );
    }

    renderTemplates() {
        const { dataActivity } = this.props;
        const list = get(dataActivity, 'response.targets', []);
        return (
            <Menu
                mode="inline"
                selectedKeys={[String(this.state.skinId)]}
                openKeys={[String(this.state.templateId)]}
                onOpenChange={(item) => {
                    this.onOpenChange(item);
                }}
            >
                {list.map((tpl) => (
                    <Menu.SubMenu key={String(tpl.id)} title={tpl.name}>
                        {this.state.templateId == tpl.id && (
                            <Menu.ItemGroup title="皮肤列表">
                                {this.getSkinsData().map((skin) => [
                                    <Menu.Item
                                        key={skin.id}
                                        style={{ height: 80 }}
                                        onClick={this.handleClick}
                                    >
                                        <Tooltip
                                            title={skin.name}
                                            color="blue"
                                            placement="right"
                                        >
                                            <img
                                                src={getAbsoluteStaticUrl(
                                                    skin.preview.uri
                                                )}
                                                alt="preview"
                                                width={200}
                                            />
                                        </Tooltip>
                                    </Menu.Item>,
                                    <Menu.Divider></Menu.Divider>,
                                ])}
                            </Menu.ItemGroup>
                        )}
                    </Menu.SubMenu>
                ))}
            </Menu>
        );
    }

    render() {
        const { dataActivity } = this.props;
        const { response } = dataActivity;
        return (
            <Layout style={{ height: '100%' }}>
                <Layout.Sider
                    theme="light"
                    width={320}
                    style={{
                        overflow: 'false',
                        height: '100%',
                    }}
                >
                    <ReactCustomScrollBars
                        autoHide
                        autoHideTimeout={600}
                        autoHideDuration={400}
                    >
                        <Toolbar onSearch={this.handleSearch} />
                        <Spin
                            spinning={isFetchLoading(dataActivity)}
                            tip="Loading..."
                        >
                            {this.renderTemplates()}
                        </Spin>
                        <Pagination
                            current={
                                startToPage(response.start, response.count) || 1
                            }
                            total={response.total || 0}
                            onChange={this.handlePageChange}
                            simple
                        />
                    </ReactCustomScrollBars>
                </Layout.Sider>
                {this.getTargetSkinData() && (
                    <Layout style={{ height: '100%' }}>
                        <Layout.Header
                            theme="light"
                            style={{
                                height: 120,
                                padding: '10px 30px 0px',
                                background: 0xffffff,
                            }}
                        >
                            {this.renderHeader()}
                        </Layout.Header>
                        <Layout.Content theme="light" style={{ padding: 30 }}>
                            <FileView
                                templateId={this.state.templateId}
                                skinId={this.state.skinId}
                            ></FileView>
                            <StepsView
                                templateId={this.state.templateId}
                                skinId={this.state.skinId}
                            ></StepsView>
                        </Layout.Content>
                    </Layout>
                )}
            </Layout>
        );
    }
}

Index.propTypes = {
    dispatch: PropTypes.func.isRequired,
    dataActivity: PropTypes.object.isRequired,
};

function stateToProps(state) {
    const { dataSkin, dataProperty, dataActivity, dataWebsocket } = state;
    return { dataSkin, dataProperty, dataActivity, dataWebsocket };
}

export default withRouter(connect(stateToProps)(Index));
