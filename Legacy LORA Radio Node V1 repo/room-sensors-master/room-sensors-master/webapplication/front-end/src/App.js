import React, { Component } from 'react';
import Header from './components/header/header';
import Heatmap from './components/heatmap/heatmap';
import Sensor from './components/sensor/sensor';
import Login from './components/login/login';
import Toolbar from './components/toolbar/toolbar';
import {BrowserRouter as Router, Route, Switch} from 'react-router-dom';
import { Container } from 'react-bootstrap';
import history from './history/history';

class App extends Component {

  render() {
    return (
      <Container>
        <Header />
        <Router history={history}>
          <Toolbar />
          <Switch>
            <Route exact path="/" component={Heatmap} />
            <Route exact path="/sensor/:serialID" component={Sensor} />
            <Route exact path="/login" component={Login} />
          </Switch>
        </Router>
      </Container>
    )
  }
}

export default App;
