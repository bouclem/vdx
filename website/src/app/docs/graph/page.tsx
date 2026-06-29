import DocPage from "@/components/DocPage";

export default function GraphDoc() {
  return (
    <DocPage title="Graph Module">
      <p>
        The <code>graph</code> module provides plotting and data visualization.
        It generates SVG vector graphics files — no external dependencies required.
        SVG files can be opened in any web browser or vector graphics editor.
      </p>

      <h2>Overview</h2>
      <p>
        All functions are accessed via the <code>graph.</code> prefix.
        Plot configuration functions (<code>graph.title</code>, <code>graph.xlabel</code>,{" "}
        <code>graph.ylabel</code>) set labels for the next plot you create.
        Use <code>graph.save(path)</code> to write the SVG to disk, or{" "}
        <code>graph.show()</code> to open it in the default viewer.
      </p>

      <h2>Plot Functions</h2>
      <table>
        <thead>
          <tr>
            <th>Function</th>
            <th>Arguments</th>
            <th>Description</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><code>graph.scatter(xs, ys)</code></td>
            <td>array, array</td>
            <td>Scatter plot from two numeric arrays (must be same length)</td>
          </tr>
          <tr>
            <td><code>graph.line(xs, ys)</code></td>
            <td>array, array</td>
            <td>Line chart connecting (x, y) points with a polyline</td>
          </tr>
          <tr>
            <td><code>graph.bar(labels, values)</code></td>
            <td>array, array</td>
            <td>Vertical bar chart with category labels</td>
          </tr>
          <tr>
            <td><code>graph.hist(data, [bins])</code></td>
            <td>array, [int]</td>
            <td>Histogram with configurable bin count (default: 10)</td>
          </tr>
        </tbody>
      </table>

      <h2>Configuration Functions</h2>
      <table>
        <thead>
          <tr>
            <th>Function</th>
            <th>Arguments</th>
            <th>Description</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><code>graph.title(text)</code></td>
            <td>string</td>
            <td>Set the title for the next plot</td>
          </tr>
          <tr>
            <td><code>graph.xlabel(text)</code></td>
            <td>string</td>
            <td>Set the x-axis label for the next plot</td>
          </tr>
          <tr>
            <td><code>graph.ylabel(text)</code></td>
            <td>string</td>
            <td>Set the y-axis label for the next plot</td>
          </tr>
        </tbody>
      </table>

      <h2>Output Functions</h2>
      <table>
        <thead>
          <tr>
            <th>Function</th>
            <th>Arguments</th>
            <th>Description</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><code>graph.save(path)</code></td>
            <td>string</td>
            <td>Save the current plot as an SVG file</td>
          </tr>
          <tr>
            <td><code>graph.show()</code></td>
            <td>none</td>
            <td>Open the current plot in the default SVG viewer</td>
          </tr>
        </tbody>
      </table>

      <h2>Examples</h2>

      <h3>Scatter plot</h3>
      <pre><code>{`let xs = [1, 2, 3, 4, 5, 6, 7, 8];
let ys = [2, 4, 5, 4, 5, 7, 8, 9];

graph.title("My Scatter Plot");
graph.xlabel("X");
graph.ylabel("Y");
graph.scatter(xs, ys);
graph.save("plot.svg");`}</code></pre>

      <h3>Line chart</h3>
      <pre><code>{`let months = [1, 2, 3, 4, 5, 6];
let temps = [5, 8, 12, 18, 22, 25];

graph.title("Temperature Over Time");
graph.xlabel("Month");
graph.ylabel("Temperature (C)");
graph.line(months, temps);
graph.save("temp.svg");`}</code></pre>

      <h3>Bar chart</h3>
      <pre><code>{`let labels = ["Mon", "Tue", "Wed", "Thu", "Fri"];
let values = [23, 45, 12, 67, 34];

graph.title("Daily Sales");
graph.xlabel("Day");
graph.ylabel("Units Sold");
graph.bar(labels, values);
graph.save("sales.svg");`}</code></pre>

      <h3>Histogram</h3>
      <pre><code>{`let data = [1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5];

graph.title("Data Distribution");
graph.xlabel("Value");
graph.ylabel("Frequency");
graph.hist(data, 5);
graph.save("hist.svg");`}</code></pre>

      <h2>Complete example</h2>
      <pre><code>{`// Generate and save multiple plot types
let xs = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
let ys = [2, 4, 5, 4, 5, 7, 8, 9, 10, 12];

graph.title("Scatter Plot Demo");
graph.xlabel("X Axis");
graph.ylabel("Y Axis");
graph.scatter(xs, ys);
graph.save("scatter.svg");
print("Scatter plot saved");

let temps = [10, 12, 15, 18, 22, 25, 28, 27, 24, 20, 16, 12];
let months = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12];

graph.title("Monthly Temperature");
graph.xlabel("Month");
graph.ylabel("Temperature (C)");
graph.line(months, temps);
graph.save("temperature.svg");
print("Line chart saved");`}</code></pre>

      <h2>Error handling</h2>
      <ul>
        <li>All plot functions require numeric arrays — non-numeric values throw an error</li>
        <li><code>graph.scatter()</code> and <code>graph.line()</code> require xs and ys to be the same length</li>
        <li><code>graph.hist()</code> requires bins to be a positive integer</li>
        <li><code>graph.save()</code> and <code>graph.show()</code> throw an error if no plot has been created</li>
        <li><code>graph.save()</code> throws an error if the file cannot be written</li>
      </ul>
    </DocPage>
  );
}
