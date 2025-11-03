import store from '../store/store';
import { setSensors, setLoading, setCoord, setSensor, setToken } from '../actions/actions';

export const sensorsLink = process.env.REACT_APP_SENSORLINK;
export const credsLink = process.env.REACT_APP_CREDLINK;

export const getSensors = (link) => {
    store.dispatch(setLoading());
    fetch(link)
    .then(res => res.json())
    .then(result => store.dispatch(setSensors(result)));
    console.log("sensors updated");
}

export const addSensor = (link, sensor) => {
    store.dispatch(setLoading());
    fetch(link, 
        {
            method: 'POST',
            body: JSON.stringify(sensor),
            headers: {
                'Content-Type': 'application/json',
                'Authorization': 'Token ' + store.getState().loginReducer.token
            }
        }    
    )
    .then(res => res.json())
    .then(() => store.dispatch(setCoord(null)))
    .then(() => getSensors(link));
}

export const getSensor = (link, key, navigate) => {
    console.log(key);
    store.dispatch(setLoading());
    fetch(link+key+'/')
    .then(res => res.json())
    .then(result => {
        store.dispatch(setSensor(result))
    })
    .then(() => {
        if (navigate) {
            navigate(key);
        }
    });
}

export const deleteSensor = (link, sensor, navigate) => {
    store.dispatch(setLoading());
    fetch(link+sensor.serialID+"/", {
        method: "DELETE",
        headers: {
            'Content-Type': 'application/json',
            'Authorization': 'Token ' + store.getState().loginReducer.token
        }
    })
    .then(() => navigate())
    .then(() => getSensors(sensorsLink))
    .then(() => store.dispatch(setSensor(null)));
    
}

export const getToken = (username, password, navigate) => {
    store.dispatch(setLoading(true));
    const credStorage = localStorage;

    fetch(credsLink, {
        method: 'POST',
        body: JSON.stringify({ 'username': username, 'password': password }),
        headers: {
            'Content-Type': 'application/json'
        }
    })
        .then( res => res.json() )
        .then( result => {
            store.dispatch(setLoading(false));

            if(result.token) {
                store.dispatch(setToken(result.token));
                credStorage.setItem('token', result.token);
                navigate();
            }
        })
        .catch(e => console.log(`utils.getToken: ${e.message}`));
}