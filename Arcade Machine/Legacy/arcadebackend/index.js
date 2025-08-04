import express, {json} from "express"
import { exec } from "child_process"
import cors from "cors"
import path from "path"
import fs from "fs"
import dotenv from "dotenv";
const app = express();
dotenv.config();

const PORT = process.env.PORT
const DIRECTORY = process.env.REPO_DIRECTORY
let isRunning = false
app.use(json())
app.use(cors());

process.chdir(DIRECTORY);

app.post('/executeShortcut', (req, res) => {
    if (isRunning) {
        res.status(500).send('Shortcut already running');
        return;
    }
    isRunning = true;

    const { path: relativePath, antiMicroPath } = req.body;

    // Ensure the required parameters are provided
    if (!relativePath || !antiMicroPath) {
        res.status(400).send('Missing required parameters');
        isRunning = false;
        return;
    }

    const newDirectory = path.join(DIRECTORY, antiMicroPath);

    // process.chdir(newDirectory);
    
    // exec(`"../../../controller-select/load_profile.py"`, (error, stdout, stderr) => {
    //     if (error) {
    //         console.error(`Error executing shortcut: ${error}`);
    //         res.status(500).send('Error executing shortcut');
    //         isRunning = false;
    //         process.chdir(DIRECTORY); // Ensure we switch back to the original directory
    //         return;
    //     }

    //     if (stderr) {
    //         console.error(`Error output from shortcut: ${stderr}`);
    //         res.status(500).send('Error executing shortcut');
    //         isRunning = false;
    //         process.chdir(DIRECTORY); // Ensure we switch back to the original directory
    //         return;
    //     }

    //     console.log('Output from shortcut: successful');

    //     // Switch back to the original directory
    //     process.chdir(DIRECTORY);

        const absolutePath = path.join(DIRECTORY, relativePath);
        console.log(`Executing shortcut: ${absolutePath}`);
        exec(`start "" "${absolutePath}"`, (error, stdout, stderr) => {
            if (error) {
                console.error(`Error executing shortcut: ${error}`);
                res.status(500).send('Error executing shortcut');
                isRunning = false;
                return;
            }

            if (stderr) {
                console.error(`Error output from shortcut: ${stderr}`);
                res.status(500).send('Error executing shortcut');
                isRunning = false;
                return;
            }

            console.log(`Output from shortcut: ${stdout}`);
            isRunning = false;
            res.status(200).send('Shortcut executed successfully');
        });
    // });
});



app.get("/getGames", (req, res) => {
    try {
        const esrb = req.query.esrb;
        const ratings = ["E", "E10+", "T", "M", "AO"];
        const games = [];
        const basePath = "./move";
        let breakloop = false;

        

        // Ensure the esrb rating is valid
        if (!ratings.includes(esrb)) {
            return res.status(400).json({ error: "Invalid ESRB rating" });
        }
        // Loop through the ratings up to the selected ESRB rating
        for (const rating of ratings) {
            const ratingPath = `${basePath}/${rating}`;

            if (breakloop){
                break;
            }
            // Stop looping if we reached the selected ESRB rating
            if (rating === esrb){
                console.log(rating, esrb)
                breakloop = true;
            } 

            // Read the games directory for the current rating
            const gamesDir = fs.readdirSync(ratingPath, { withFileTypes: true });

            gamesDir.forEach(gameDirent => {
                if (gameDirent.isDirectory()) {
                    const game = gameDirent.name;
                    const gamePath = `${ratingPath}/${game}`;
                    const gameContents = fs.readdirSync(gamePath, { withFileTypes: true });

                    let exepath = "";

                    gameContents.forEach(contentDirent => {
                        if (contentDirent.isDirectory()) {
                            // console.log(contentDirent.name);
                            const contentPath = `${gamePath}/${contentDirent.name}`;

                            const folderContents = fs.readdirSync(contentPath);

                            folderContents.forEach(file => {
                                if (file.endsWith(".exe") && !file.startsWith("UnityCrashHandler")) {
                                    exepath = `${rating}/${game}/${contentDirent.name}/${file}`;
                                }
                            });
                        }
                    });

                    games.push({
                        game: game,
                        folderContents: gameContents.map(content => content.name),
                        exepath: exepath,
                        antiMicroPath: `${rating}/${game}`
                    });
                }

            });
        }

        res.status(200).json({ path: basePath, games: games });
    } catch (error) {
        res.status(404).json({ error: error.message });
    }
});


app.listen(PORT, () => {
    console.log(`Server is running on port: ${PORT}`)
})

export default app;
