import axios from "axios";
const Button = ({ game, path }) => {
  const launchGame = () => {
    axios.post("http://localhost:3001/executeShortcut", {
      path: path,
    });
  };

  return (
    <button type="button" onClick={launchGame}>
      {game}
    </button>
  );
};

export default Button;
