import React, { Component } from 'react';
import { connect } from "react-redux";
import {withRouter} from 'react-router-dom';
import store from '../../store/store';
import { Dropdown, Button, ButtonToolbar } from 'react-bootstrap';
import { setFloor, setAdmin, setToken } from '../../actions/actions';
import DeleteForm from "../deleteForm/deleteForm";

const mapStateToProps = state => {
    return {
        adminMode: state.rootReducer.adminMode,
        token: state.loginReducer.token,
        floor: state.rootReducer.floor
    };
};

const mapDispatchToProps = {
    setFloor,
    setAdmin
};

class ConnectedToolbar extends Component {
    constructor(props) {
        super(props)

        this.state = {
            deleting: false
        };

        this.changeFloor = this.changeFloor.bind(this);
        this.setAdmin = this.setAdmin.bind(this);
        this.goToMap = this.goToMap.bind(this);
        this.toggleDelete = this.toggleDelete.bind(this);
        this.logout = this.logout.bind(this);
        this.login = this.login.bind(this);
    }

    componentDidMount() {
        if (localStorage.getItem('token') && !this.props.token) {
            const token = localStorage.getItem('token');
            store.dispatch(setToken(token));
        }
    }

    componentWillReceiveProps() {
        console.log("new props");
    }

    changeFloor(eventKey) {
        this.props.setFloor(eventKey);
    }

    setAdmin() {
        this.props.setAdmin();
    }

    goToMap() {
        this.props.history.push('/');
    }

    toggleDelete() {
        console.log("handle delete");
        this.setState({
            deleting: !this.state.deleting
        });
    }

    logout() {
        const storage = localStorage;
        storage.removeItem('token');
        store.dispatch(setToken(null));

        if (this.props.adminMode) {
            this.props.setAdmin();
        }
    }

    login() {
        this.props.history.push('/login');
    }

    render() {
        return([
            <ButtonToolbar key="ButtonToolbar">
                {/* Show back button instead of Floor select if in Sensor Details view */}
                {this.props.history.location.pathname.includes('/sensor/') ?
                    <Button variant="secondary" onClick={this.goToMap}>
                        <i className="fas fa-arrow-left"></i>Back to map
                    </Button>
                :
                    <Dropdown title="Choose floor">
                        <Dropdown.Toggle variant="success" id="dropdown-basic">
                            Floor {this.props.floor}
                        </Dropdown.Toggle>
                        <Dropdown.Menu>
                            <Dropdown.Item eventKey="one" onSelect={this.changeFloor} active={this.props.floor === "one"}>Floor 1</Dropdown.Item>
                            <Dropdown.Item eventKey="two" onSelect={this.changeFloor} active={this.props.floor === "two"}>Floor 2</Dropdown.Item>
                            <Dropdown.Item eventKey="hub" onSelect={this.changeFloor} active={this.props.floor === "hub"}>Hub</Dropdown.Item>
                        </Dropdown.Menu>
                    </Dropdown>
                }

                {this.props.adminMode && this.props.history.location.pathname.includes('/sensor/') ? <Button variant="danger" onClick={this.toggleDelete}>Delete</Button> : null}
                {this.props.token ? <Logged adminMode={this.props.adminMode} setAdmin={this.setAdmin} logout={this.logout}  /> : <NotLogged login={this.login} />}
            </ButtonToolbar>,
            <DeleteForm key="deleteConfirm" show={this.state.deleting} handleClose={this.toggleDelete} 
                sensorName={this.props.selectedSensor ? this.props.selectedSensor.name : null} />
        ]);
    }
}

function Logged(props) {
    return([
        <Button key="adminMode" variant={props.adminMode ? "success" : "warning"} onClick={props.setAdmin} className="btn-admin">
            Admin Mode {props.adminMode ? "on" : "off"}
        </Button>,
        <Button key="logout" variant="info" onClick={props.logout}>
            Logout
        </Button>
    ])
}

function NotLogged(props) {
    return(
        <Button key="login" onClick={props.login} className="btn btn-success pull-right" >
            Login
        </Button>
    )
    
}

const Toolbar = withRouter(connect(mapStateToProps, mapDispatchToProps)(ConnectedToolbar));
export default Toolbar;