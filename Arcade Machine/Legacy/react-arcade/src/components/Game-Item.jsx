import "./App.scss";

const Item = (item) => {
  return (
    <div className={"game-item"}>
      {/* Game Title taken from folder name */}
      {item.item.game} 
      {/* Game image taken from folder to go here. do we need to specify file type? eg jpeg or png only? */}
    </div>
  );
};

export default Item