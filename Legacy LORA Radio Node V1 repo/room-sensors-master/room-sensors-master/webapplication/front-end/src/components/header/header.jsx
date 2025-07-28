import React, { Component } from 'react';
import { Row, Col } from 'react-bootstrap';

class Header extends Component {
    constructor(props) {
        super(props)

        this.clicker = this.clicker.bind(this);
    }

    clicker(event) {
        console.log(event.target);
    }
    render() {
        return (
            <Row className="page-header">
                <Col xs={12}>
                    <h2 onClick={this.clicker}>Otago Polytechnic - Heatmap</h2>
                </Col>
            </Row>
        );
    }
}

export default Header;