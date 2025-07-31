import { createStore, combineReducers } from 'redux';
import { composeWithDevTools } from 'redux-devtools-extension';
import rootReducer from '../reducers/rootReducer';
import loginReducer from '../reducers/loginReducer';

const store = createStore(
    combineReducers({
        rootReducer,
        loginReducer
    }),
    composeWithDevTools()
);

export default store;