import { SET_TOKEN } from "../actions/action-types";

const initialState = {
    token: null
}


const loginReducer = (state = initialState, action) => {
    const newState = Object.assign({}, state); //makes sure there is always a new object(state) when something changes, rather then the same object with new property;

    switch (action.type) {
        case SET_TOKEN:
            newState.token = action.payload;
            return newState;
        default:
            return state; 
    };
};

export default loginReducer;