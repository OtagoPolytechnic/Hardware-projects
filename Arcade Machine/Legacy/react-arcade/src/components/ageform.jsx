import { useEffect, useRef, useState } from "react"
import { getGames } from "../utils/Arcade-Backend-Functions"
const Ageform = ({setGames, setPath}) => { 
    const [index, setIndex] = useState(0)
    const indexRef = useRef(index);
    const ratings = ["E", "E10+", "T", "M", "AO"]

    useEffect(() => {
        // Update the ref whenever age changes
        indexRef.current = index;
    }, [index]);

    useEffect(() => {
        getGames(setGames, setPath, "AO")
    },[])

    useEffect(() => {
        const keydown = (e) => {
            if(e.key === "ArrowUp"){
                if(indexRef.current >= ratings.length -1){
                    setIndex(4)
                } else {
                setIndex((prevIndex) => prevIndex + 1)
                }
            }
            if(e.key === "ArrowDown"){
                if(indexRef.current <= 0){
                    setIndex(0)
                } else {
                setIndex((prevIndex) => prevIndex - 1)
                }
            }
            if(e.key === "Enter"){
                getGames(setGames, setPath, ratings[indexRef.current])
            }
        }
    window.addEventListener('keydown', keydown);

    // Clean up event listener when component unmounts
    return () => {
        window.removeEventListener('keydown', keydown);
    };
    }, []);

    return(
        <div className="ageCheck">
            <h1>Select Maximum ESRB Rating</h1>
            <h2>^</h2>
            <h2>{ratings[index]}</h2>
            <h2>v</h2>
            <p>Press Enter to submit ESRB</p>
        </div>
    )
}

export default Ageform