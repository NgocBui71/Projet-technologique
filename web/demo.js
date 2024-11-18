Module.onRuntimeInitialized = () => { start(); }

const EMPTY = 0;
const WHITE = 1;
const BLACK = 2;
const UNCONSTRAINED = -1;
const FULL = 0;
const ORTHO = 1;
const FULL_EXCLUDE = 2;
const ORTHO_EXCLUDE = 3;

let col2str = [" ", "□", "■"];
let num2str = [
    ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"],     // empty
    ["🄋", "➀", "➁", "➂", "➃", "➄", "➅", "➆", "➇", "➈"],  // white
    ["⓿", "❶", "❷", "❸", "❹", "❺", "❻", "❼", "❽", "❾"],     // black
];


function square2str(n, c) {
    if (n == UNCONSTRAINED)
        return col2str[c];
    else
        return num2str[c][n];
}


function printGame(g) {
    var text = "";
    var nb_rows = Module._nb_rows(g);
    var nb_cols = Module._nb_cols(g);
    for (var row = 0; row < nb_rows; row++) {
        for (var col = 0; col < nb_cols; col++) {
            var n = Module._get_constraint(g, row, col);
            var c = Module._get_color(g, row, col);
            var status = Module._get_status(g, row, col);
            text += square2str(n, c);
        }
        text += "\n";
    }

    // put this text in <div> element with ID 'result'
    var elm = document.getElementById('result');
    elm.innerHTML = text;
}

function start() {
    console.log("call start routine");
    // var g = Module._new_default();
    var g = Module._new_random(6, 6, false, FULL, 0.6, 0.5);
    Module._play_move(g, 0, 0, BLACK);
    printGame(g);
    Module._delete(g);
}

