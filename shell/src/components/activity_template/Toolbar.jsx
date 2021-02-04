import React from 'react';
import { Form, Input, Button, Row, Col } from 'antd';
import { SearchOutlined, PlusCircleOutlined } from '@ant-design/icons';

class Toolbar extends React.Component {
    // static propTypes = {
    //   form: React.PropTypes.object,
    //   onSearch: React.PropTypes.func.isRequired,
    //   onAdd: React.PropTypes.func.isRequired,
    // };

    handleSearch = (values) => {
        this.props.onSearch(values);
    };

    render() {
        return (
            <Form
                className="search-form"
                onFinish={this.handleSearch}
                style={{ padding: '10px 15px 0px' }}
            >
                <Form.Item name="name">
                    <Row type="flex" align="middle">
                        <Col>
                            <Input placeholder="Search" autoComplete="off" />
                        </Col>
                        <Col>
                            <Button
                                type="primary"
                                icon={<SearchOutlined />}
                                htmlType="submit"
                            >
                                Search
                            </Button>
                        </Col>
                    </Row>
                </Form.Item>
            </Form>
        );
    }
}

export default Toolbar;
