import React, { Component } from 'react';
import { connect } from "react-redux";
import ReactLoading from 'react-loading';
import ReactTooltip from 'react-tooltip';
import * as utils from '../../utils/utils';
import * as d3 from 'd3';
import './sensor.css';

const mapStateToProps = state => {
    return {
        loading: state.rootReducer.loading,
        floor: state.rootReducer.floor,
        adminMode: state.rootReducer.adminMode,
        selectedSensor: state.rootReducer.selectedSensor,
        sensors: state.rootReducer.sensors
    };
};

class ConnectedSensor extends Component {
    constructor(props) {
        super(props)

        this.drawChart = this.drawChart.bind(this);
        this.navigate = this.navigate.bind(this);
    }

    componentWillMount() {
        let pathname = this.props.history.location.pathname;

        //Checking for the trailing slash at the end of the pathname
        if (pathname[pathname.length - 1] == "/") {
            pathname = pathname.substring(0, pathname.length -1);
        }

        const lastSlashIndex = pathname.lastIndexOf('/');
        const sensorId = parseInt(pathname.substring(lastSlashIndex + 1));
        
        if (!this.props.selectedSensor) {
            utils.getSensor(utils.sensorsLink, sensorId, null);
        }
    }

    navigate(serialID) {
        this.props.history.push('/sensor/' + serialID);
    }

    componentDidMount() {
        this.drawChart();
    }

    componentDidUpdate() {
        d3.select("#graph").select("svg").remove();
        this.drawChart();
    }

    drawChart() {
        try {
            const svg = d3.select("#graph")
                        .append("svg");
            
            const margin = {top: 20, right: 30, bottom: 30, left: 40};
            const width = document.getElementsByClassName("container")[0].clientWidth - margin.left - margin.right;
            const height = ((width + margin.top + margin.bottom) * 0.5);
            
            //Setting dimensions of the svg element
            svg.attr("width", width + margin.left + margin.right)
                .attr("height", height + margin.top + margin.bottom);

            const x = d3.scaleTime().range([0, width]);
            const y = d3.scaleLinear().range([height, 0]);

            const xAxis = d3.axisBottom(x);
            const yAxis = d3.axisLeft(y);

            const zoom = d3.zoom()
                        .scaleExtent([1, 32])
                        .translateExtent([[0, 0], [width, height]])
                        .extent([[0, 0], [width, height]])
                        .on("zoom", zoomed);

        
            const area = d3.area()
                    .curve(d3.curveMonotoneX)
                    .x(function(d) { return x(new Date(d.timestamp)); })
                    .y0(height)
                    .y1(function(d) { return y(d.temperature) });
        
            svg.append("defs").append("clipPath")
                .attr("id", "clip")
                .append("rect")
                .attr("width", width)
                .attr("height", height);
            
            const g = svg.append("g")
                .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

            const data = this.props.selectedSensor.history;

            x.domain(d3.extent(data, function(d) { return new Date(d.timestamp) }));
            y.domain([15, d3.max(data, function(d) { return d.temperature })]);


            g.append("path")
                .datum(data)
                .attr("class", "area")
                .attr("d", area);

            console.log(area);

            //Add data points
            // g.selectAll("circles")
            //     .data(data)
            //     .enter()
            //     .append("circle")
            //         .attr("fill", "red")
            //         .attr("stroke", "none")
            //         .attr("cx", function(d) {return x(new Date(d.timestamp))})
            //         .attr("cy", function(d) {return y(d.temperature)})
            //         .attr("r", 2)
            //         .append("a")
            //         .attr("class", "tooltip anchor")
            //         .attr("data-tip", "")
            //         .attr("data-for", function(d) {return d.timestamp});
            
            g.append("g")
                .attr("class", "axis axis--x")
                .attr("transform", "translate(0, " + height + ")")
                .call(xAxis);

            g.append("g")
                .attr("class", "axis axis--y")
                .call(yAxis);

            const d0 = new Date(data[0].timestamp);
            const d1 = new Date(data[data.length - 1].timestamp);

            svg.call(zoom).transition()
                .duration(1500)
                .call(zoom.transform, d3.zoomIdentity
                    .scale(width / (x(d1) - x(d0)))
                    .translate(-x(d0), 0));

            function zoomed() {
                const t = d3.event.transform;
                const xt = t.rescaleX(x);
                const yt = t.rescaleY(y);
                g.select(".area").attr("d", area.x(function(d) { return xt(new Date(d.timestamp)); }));
                // g.selectAll("circle")
                //     .attr("cx", function(d) {return xt(new Date(d.timestamp))}); //re-positioning circles

                g.select(".axis--x").call(xAxis.scale(xt));
            }        

        } catch(e) {
            if (e instanceof TypeError) {
                console.log("Error: " + e);
            }
        }
    }

    render() {
        const sensor = this.props.selectedSensor;
        if (!sensor || this.props.loading) {
            return <ReactLoading className="busy wrapper" type="spinningBubbles" color="grey" height={100} width={100} />
        }
        return ([
            <div key="sensor" className="sensor-graph">
                <h1>{this.props.selectedSensor.sensor.name} - {this.props.selectedSensor.sensor.serialID}</h1>
                <div className="graph" id="graph">
                    {/* {this.props.selectedSensor.history.map((history, index) => (
                        <ReactTooltip key={index} id={history.timestamp} type='info' effect='solid'>
                            <span className='reading'>Temp: {history.temperature}</span>
                        </ReactTooltip>
                    ))} */}
                </div>
            </div>
        ])
    }
}

const Sensor = connect(mapStateToProps)(ConnectedSensor);

export default Sensor;