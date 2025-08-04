import React, { useEffect, useState } from "react";
import Item from "./Game-Item";
import { launchGame } from "../utils/Arcade-Backend-Functions";
import "./App.scss";

const Carousel = ({ items, setActive, path }) => {
  const [index, setIndex] = useState(0);
  let SnowSlam = require("../assets/img/snowSlam.png"); //Game 1
  let SacredKrow = require("../assets/img/sacredKrow.png"); //Game 2
  let Image3 = require("../assets/img/comingSoon.gif"); //Game 3
  let Image4 = require("../assets/img/comingSoon.gif"); // Game 4

  let imgarr = [SnowSlam, SacredKrow,Image3,Image4]
  const handlePrevious = () => {
    const newIndex = index - 1;
    setIndex(newIndex < 0 ? items.length - 1 : newIndex);

    // Print all items in the array to the console
    console.log("Items in the array:");
    items.forEach((item, idx) => {
    });
  };

  useEffect(() => {
    console.log(`${path}/${items[0].exepath}`)
  }, [items])
  const handleNext = () => {
    const newIndex = index + 1;
    setIndex(newIndex >= items.length ? 0 : newIndex);
  };

  useEffect(() => {
    setActive(items[index]);
  }, [index, items, setActive]);

  useEffect(() => {
    const handleKeyDown = (event) => {
      if (event.key === "ArrowLeft") {
        handlePrevious();
      } else if (event.key === "ArrowRight") {
        handleNext();
      } else if (event.key === "Enter"){
        launchGame(`${path}/${items[index].exepath}`, `${path}/${items[index].antiMicroPath}`)
      }
    };

    window.addEventListener("keydown", handleKeyDown);
    return () => {
      window.removeEventListener("keydown", handleKeyDown);
    };
  }, [index, items]);

  // What is displayed
  return (
    <div className="carousel">
      <div className="gameSelector">
        <button onClick={handlePrevious} className="prevButton">
          Previous HIDDEN
        </button>
        <div className="gameTile">
          <Item item={items[index]} />
          <img src={imgarr[index]} className="gameImg" />
        </div>
        <button onClick={handleNext} className="nextButton">
          Next HIDDEN
        </button>
      </div>

      <div className="launchBar">
        <button
          onClick={() => launchGame(`${path}/${items[index].exepath}`, `${path}/${items[index].antiMicroPath}`)}
          className="launchButton"
        >
          launch HIDDEN
        </button>
      </div>
    </div>
  );
};

export default Carousel;
