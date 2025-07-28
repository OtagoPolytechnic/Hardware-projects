import React, { Component } from 'react';
import Sensors from '../sensors/sensors';

import { setCoord } from '../../actions/actions';
import AddForm from '../addForm/addForm';
import { connect } from "react-redux";
import './heatmap.css';

const mapStateToProps = state => {
    return {
        floor: state.rootReducer.floor,
        adminMode: state.rootReducer.adminMode
    };
};

const mapDispatchToProps = {
    setCoord
};

class ConnectedHeatmap extends Component {
    constructor(props) {
        super(props)

        this.getCoord = this.getCoord.bind(this);
    }

    getCoord(data) {
        if (this.props.adminMode) {
            console.log("getting coordinates");
            this.props.setCoord({
                xCoord: data.nativeEvent.offsetX,
                yCoord: data.nativeEvent.offsetY
            });
        }
    }

    render() {
        return ([
            <div id="map" key="map" className={this.props.floor.toString()} onClick={this.getCoord}></div>,
            <Sensors key="sensors" />,
            <AddForm key="addForm" />
        ]);
    };
};

const Heatmap = connect(mapStateToProps, mapDispatchToProps)(ConnectedHeatmap);

export default Heatmap;