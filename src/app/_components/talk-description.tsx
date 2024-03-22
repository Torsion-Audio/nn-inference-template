import { Post } from "@/interfaces/post";
import { PostPreview } from "./post-preview";

type Props = {
  posts: Post[];
};

export function TalkDescription({ posts }: Props) {

    const descriptionText = (
        <>
          <p style={{ marginBottom: '20px' }}>
          In upcoming audio processing innovations the intersection of neural networks and real-time environments is set to play a decisive role. Our recent experience of implementing neural timbre transfer technology in a real-time setting has presented us with diverse challenges. Overcoming them has provided us with significant insights into the practicalities of inferencing neural networks inside an audio plugin.
          </p>
          <p style={{ marginBottom: '20px' }}>
          This talk presents a pragmatic approach: Starting with a trained model, we guide you through the necessary steps for inferencing the model in a real-time environment. On our way we delve into the critical aspect of maintaining real-time safety, share proven strategies to ensure a seamless and uninterrupted signal flow. Moreover, we address the delicate balance between latency, performance, and stability. For this we utilize three different inference engines: libtorch, tensorflow-lite and onnxruntime. While the in-house solutions for the popular machine learning frameworks PyTorch and TensorFlow, seem obvious choices, sometimes other engines may be better suited for certain use cases. By contrasting the characteristics of the engines, we hope to simplify your decision-making process.
          </p>
        </>
      );


    return (
        <section>
        <h2 className="mb-8 text-5xl md:text-7xl font-bold tracking-tighter leading-tight">
            Talk description
        </h2>
        <div className="mb-20 md:mb-28">
            <p className="text-lg leading-relaxed mb-4">{descriptionText}</p>
        </div>
        </section>
    );
}
