import React, { Component } from 'react';
import {Form, Button} from 'react-bootstrap';
import * as utils from '../../utils/utils';
import './login.css';


class Login extends Component {
    constructor(props) {
        super(props)

        this.state = {
            username: '',
            password: ''
        }

        this.changePass = this.changePass.bind(this);
        this.changeUser = this.changeUser.bind(this);
        this.submit = this.submit.bind(this);
        this.navigate = this.navigate.bind(this);
    }

    changeUser(event) {
        this.setState({
            username: event.target.value
        })
    }

    changePass(event) {
        this.setState({
            password: event.target.value
        })
    }

    submit(event) {
        event.preventDefault();

        if (this.state.username != '' && this.state.password != '') {
            utils.getToken(this.state.username, this.state.password, this.navigate);
        }
    }

    navigate() {
        this.props.history.push('/');
    }

    render() {
        return (
            <Form className="loginForm">
                <Form.Group controlId="loginForm">
                    <Form.Label>Username</Form.Label>
                    <Form.Control type="text" placeholder="Enter username" value={this.state.username} onChange={this.changeUser} />
                </Form.Group>
                <Form.Group controlId="formBasicPassword">
                    <Form.Label>Password</Form.Label>
                    <Form.Control type="password" placeholder="Password"  value={this.state.password} onChange={this.changePass} />
                </Form.Group>
                <Button variant="primary" type="submit" onClick={this.submit}>
                    Login
                </Button>
            </Form>
        )
    }
}

export default Login;