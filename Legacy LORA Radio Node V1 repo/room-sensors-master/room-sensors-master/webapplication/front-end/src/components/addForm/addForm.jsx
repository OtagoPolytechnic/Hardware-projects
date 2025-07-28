import React, { Component } from 'react';
import { connect } from "react-redux";
import * as utils from '../../utils/utils';
import {Form, Col, ButtonGroup, Button} from 'react-bootstrap';
import { setCoord } from '../../actions/actions';
import './addForm.css';

const mapStateToProps = state => {
    return {
        sensors: state.rootReducer.sensors,
        loading: state.rootReducer.loading,
        floor: state.rootReducer.floor,
        adminMode: state.rootReducer.adminMode,
        coord: state.rootReducer.coord
    };
};

const mapDispatchToProps = {
    setCoord
};

class ConnectedAddForm extends Component {
    constructor(props) {
        super(props)

        this.state = {
            sensorID: null,
            sensorName: null
        }

        this.handleSubmit = this.handleSubmit.bind(this);
        this.cancelAdding = this.cancelAdding.bind(this);
        this.changeSerial = this.changeSerial.bind(this);
        this.changeName = this.changeName.bind(this);
    }

    handleSubmit(event) {
        event.preventDefault();
        const sensor = {
            serialID: this.state.sensorID,
            x: this.props.coord.xCoord,
            y: this.props.coord.yCoord,
            floor: this.props.floor,
            name: this.state.sensorName
        };
        utils.addSensor(utils.sensorsLink, sensor);
    }

    changeSerial(event) {
        this.setState(
            {
                sensorID: parseInt(event.target.value)
            }
        )
    }

    changeName(event) {
        this.setState(
            {
                sensorName: event.target.value
            }
        )
    }

    cancelAdding(event) {
        event.preventDefault();
        this.props.setCoord(null);
    }

    render() {
        if (this.props.adminMode && this.props.coord) {
            return (
                <Form className="add-form" onSubmit={this.handleSubmit}>
                    <h2>Add new sensor</h2>
                    <Form.Row>
                        <Form.Group as={Col} sm="12" controlId="sensorUID">
                            <Form.Control required type="number" placeholder="Enter sensor UID" onChange={this.changeSerial} />
                        </Form.Group>
                    </Form.Row>
                    <Form.Row>
                        <Form.Group as={Col} sm="12" controlId="sensorName">
                            <Form.Control required type="text" placeholder="Enter sensor name" onChange={this.changeName} />
                        </Form.Group>
                    </Form.Row>
                    <Form.Row>
                        <Col sm="12">X-Coordinate: {this.props.coord.xCoord}</Col>
                        <Col sm="12">Y-Coordinate: {this.props.coord.yCoord}</Col>
                    </Form.Row>
                    <ButtonGroup>
                        <Button type="null" variant="danger" onClick={this.cancelAdding}>Cancel</Button>
                        <Button type="submit" variant="primary" className="pull-right" onClick={this.handleSubmit}>Submit form</Button>
                    </ButtonGroup>
                </Form>
            )
        }
        return false;
    }
}

const AddForm = connect(mapStateToProps, mapDispatchToProps)(ConnectedAddForm);

export default AddForm;