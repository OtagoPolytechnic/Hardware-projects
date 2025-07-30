import React, { Component } from 'react';
import { connect } from "react-redux";
import './sensors.css';
import * as d3 from 'd3';
import ReactTooltip from 'react-tooltip';
import { setCoord, setSensor } from '../../actions/actions';
import { withRouter } from 'react-router-dom';
import * as utils from '../../utils/utils';
import ReactLoading from 'react-loading';
import moment from 'moment';


const mapStateToProps = state => {
    return {
        sensors: state.rootReducer.sensors,
        loading: state.rootReducer.loading,
        floor: state.rootReducer.floor,
        coord: state.rootReducer.coord,
        adminMode: state.rootReducer.adminMode,
        selectedSensor: state.selectedSensor
    };
};

const mapDispatchToProps = {
    setCoord,
    setSensor,
};

class ConnectedSensors extends Component {
    constructor(props) {
        super(props)

        this.click = this.click.bind(this);
        this.hover = this.hover.bind(this);
        this.getCoord = this.getCoord.bind(this);
        this.drawMap = this.drawMap.bind(this);
        this.navigate = this.navigate.bind(this);
       
        
    }

    click(data) {
        utils.getSensor(utils.sensorsLink, data.serialID, this.navigate);
       
    }

    navigate(serialID) {
        this.props.history.push('/sensor/' + serialID);
    }

    hover(data) {
        console.log(data.co2_ppm);   
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

    drawMap() {
        const sensors = this.props.sensors.filter(sensor => sensor.floor === this.props.floor);
        console.log(sensors);
        // Create a color scheme for the 14-25 degrees temperature range
        const myScale = d3.scaleSequential(d3.interpolateRdYlBu).domain([25, 14]);

        const svg = d3.select("#map")
                    .append("svg")
                    .attr("width", 1140)
                    .attr("height", 850);

        const circles = svg
                            .selectAll("circle")
                            .data(sensors)
                            .enter()
                            .append("a")
                            .attr("data-for", function(d) {return d.serialID;})
                            .attr("data-tip", "")
                            .append("circle");

        const circleAttributes = circles
                                    .attr("cx", function(d) {return d.x;})
                                    .attr("cy", function(d) {return d.y;})
                                    .attr("r", 30)
                                    .attr("fill", function(d) {
                                        return myScale(parseInt(d.temperature));
                                    })
                                    .on("click", (d) => {this.click(d);})
                                    .on("mouseover", (d) => {this.hover(d);});
                                    
        // Rebind tooltips after trigerring anchors are added by D3
        ReactTooltip.rebuild()
    }

    componentWillMount() {
        utils.getSensors(utils.sensorsLink);
    }

    componentDidUpdate() {
        d3.select("#map").select("svg").remove();
        this.drawMap();
    }

    componentDidMount() {
        this.timer = setInterval(() => {
            utils.getSensors(utils.sensorsLink)
        }, 10000);

        this.drawMap();
    }

    componentWillUnmount() {
        clearInterval(this.timer);
    }

    render() {
        const sensors = this.props.sensors.filter(sensor => sensor.floor === this.props.floor);
        if (this.props.loading) {
            return <ReactLoading className="busy wrapper" type="spinningBubbles" color="grey" height={100} width={100} />
        }
        const moment = require('moment');
        return ([
            <div id="sensors" key="sensors" className={this.props.floor.toString()}>
                {sensors.map((sensor, index) => (
                     <ReactTooltip key={index} id={sensor.serialID.toString()} type='info' effect='solid'>
                   
                        <span className='reading'>ID: {sensor.serialID}</span>
                        <span className='reading'>Name: {sensor.name}</span>
                        <span className='reading'>Temperature: {sensor.temperature}</span>
                        <span className='reading'>Humidity: {sensor.humidity}</span>
                        <span className='reading'>CO2 : {sensor.co2_ppm}</span>
                        <span className='reading'>Time: {moment(sensor.time).format('DD/MM/YYYY HH:mm:ss')}</span>
                    </ReactTooltip>
                ))}
            </div>
            ]);
    }
}

const Sensors = withRouter(connect(mapStateToProps, mapDispatchToProps)(ConnectedSensors));

export default Sensors;