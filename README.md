# presentations

The style of the slide is defined in [template/beamerthemeMyStyle.sty](template/beamerthemeMyStyle.sty).

You can change the set of color by modifying: `TextColorBlue` and `BgColorBlue`.

You can change the length/width of the title box (in slide 1), by modifying `inner sep` and `text width`.

```tex
% ---------- Title ----------
\begin{tikzpicture}[remember picture,overlay]
    \node at (current page.center) [
        draw=TextColorBlue,
        rounded corners=8pt,
        fill=white,
        inner sep=10pt,
        text width=0.6\paperwidth,
        align=center
    ] {{\usebeamerfont{title}\color{TextColorBlue}\inserttitle\par}};
\end{tikzpicture}
```

Some figures are built in `tikz` (cf. `figures/*.tex`), they need to be compiled indenpendently.
