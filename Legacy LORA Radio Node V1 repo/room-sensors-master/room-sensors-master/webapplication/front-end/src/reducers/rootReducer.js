import { SET_SENSORS, SET_LOADING, SET_FLOOR, SET_ADMIN, SET_COORD, SET_SENSOR } from '../actions/action-types';

const initialState = {
    sensors: [],
    floor: "two",
    adminMode: false,
    coord: null,
    selectedSensor: null
}


const rootReducer = (state = initialState, action) => {
    const newState = Object.assign({}, state); //makes sure there is always a new object(state) when something changes, rather then the same object with new property;

    switch (action.type) {
        case SET_SENSORS:
            newState.sensors = action.payload;
            newState.loading = false;
            return newState;
        case SET_LOADING:
            newState.loading = action.active;
            return newState;
        case SET_FLOOR:
            newState.floor = action.payload;
            return newState;
        case SET_ADMIN:
            newState.adminMode = !state.adminMode;
            newState.coord = null;
            return newState;
        case SET_COORD:
            newState.coord = action.payload;
            return newState;
        case SET_SENSOR:
            newState.selectedSensor = action.payload;
            newState.loading = false;
            return newState;
        default:
            return state; 
    };
};

export default rootReducer;